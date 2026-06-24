#include "person_vla/PersonDetector.hpp"

#include <fstream>
#include <iostream>

namespace person_vla {

namespace {

std::vector<std::string> load_classes(const std::string& path) {
    std::vector<std::string> names;
    if (path.empty()) {
        names.emplace_back("person");
        return names;
    }

    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            names.push_back(line);
        }
    }

    if (names.empty()) {
        names.emplace_back("person");
    }
    return names;
}

cv::Rect clamp_box(const cv::Rect& box, const cv::Size& size) {
    return box & cv::Rect(0, 0, size.width, size.height);
}

}  // namespace

PersonDetector::PersonDetector(const AppConfig& config) : config_(config) {
    class_names_ = load_classes(config_.classes_path);
    hog_.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

    if (!config_.model_path.empty()) {
        net_ = cv::dnn::readNet(config_.model_path);
        using_dnn_ = !net_.empty();

        if (using_dnn_ && config_.prefer_cuda) {
            try {
                net_.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
                net_.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
            } catch (const cv::Exception&) {
                net_.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
                net_.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
            }
        }
    }
}

std::vector<Detection> PersonDetector::detect(const cv::Mat& frame) {
    if (frame.empty()) {
        return {};
    }

    if (using_dnn_) {
        try {
            return detect_with_dnn(frame);
        } catch (const cv::Exception& error) {
            std::cerr << "[warn] DNN detection failed, falling back to HOG: " << error.what() << "\n";
            using_dnn_ = false;
        }
    }

    return detect_with_hog(frame);
}

std::vector<Detection> PersonDetector::detect_with_dnn(const cv::Mat& frame) {
    cv::Mat blob = cv::dnn::blobFromImage(
        frame,
        1.0 / 255.0,
        cv::Size(config_.input_width, config_.input_height),
        cv::Scalar(),
        true,
        false
    );

    net_.setInput(blob);
    cv::Mat output = net_.forward();

    if (output.dims == 3 && output.size[1] < output.size[2]) {
        output = output.reshape(1, output.size[1]);
        cv::transpose(output, output);
    } else {
        output = output.reshape(1, output.total() / output.size[output.dims - 1]);
    }

    std::vector<cv::Rect> boxes;
    std::vector<float> scores;
    std::vector<int> class_ids;

    const float x_scale = static_cast<float>(frame.cols) / static_cast<float>(config_.input_width);
    const float y_scale = static_cast<float>(frame.rows) / static_cast<float>(config_.input_height);

    for (int row = 0; row < output.rows; ++row) {
        const float* data = output.ptr<float>(row);
        const int dimensions = output.cols;
        if (dimensions < 6) {
            continue;
        }

        const bool has_objectness = dimensions == static_cast<int>(class_names_.size()) + 5;
        const int class_offset = has_objectness ? 5 : 4;
        const float objectness = has_objectness ? data[4] : 1.0F;

        cv::Mat class_scores(1, dimensions - class_offset, CV_32FC1, const_cast<float*>(data + class_offset));
        cv::Point class_id_point;
        double max_class_score = 0.0;
        cv::minMaxLoc(class_scores, nullptr, &max_class_score, nullptr, &class_id_point);

        const int class_id = class_id_point.x;
        const std::string class_name =
            class_id >= 0 && class_id < static_cast<int>(class_names_.size()) ? class_names_[class_id] : "";
        const float confidence = objectness * static_cast<float>(max_class_score);
        const bool is_person = class_id == 0 || class_name == "person";
        if (!is_person || confidence < config_.confidence_threshold) {
            continue;
        }

        const float cx = data[0] * x_scale;
        const float cy = data[1] * y_scale;
        const float width = data[2] * x_scale;
        const float height = data[3] * y_scale;

        cv::Rect box(
            static_cast<int>(cx - width / 2.0F),
            static_cast<int>(cy - height / 2.0F),
            static_cast<int>(width),
            static_cast<int>(height)
        );

        boxes.push_back(clamp_box(box, frame.size()));
        scores.push_back(confidence);
        class_ids.push_back(class_id);
    }

    std::vector<int> keep;
    cv::dnn::NMSBoxes(boxes, scores, config_.confidence_threshold, config_.nms_threshold, keep);

    std::vector<Detection> detections;
    for (int index : keep) {
        detections.push_back({boxes[index], scores[index], class_ids[index], "person"});
    }
    return detections;
}

std::vector<Detection> PersonDetector::detect_with_hog(const cv::Mat& frame) {
    std::vector<cv::Rect> boxes;
    std::vector<double> weights;
    hog_.detectMultiScale(frame, boxes, weights, 0.0, cv::Size(8, 8), cv::Size(32, 32), 1.05, 2.0);

    std::vector<Detection> detections;
    for (std::size_t i = 0; i < boxes.size(); ++i) {
        detections.push_back({clamp_box(boxes[i], frame.size()), static_cast<float>(weights[i]), 0, "person"});
    }
    return detections;
}

}  // namespace person_vla

