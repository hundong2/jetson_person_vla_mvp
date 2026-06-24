#include "person_vla/ActionPolicy.hpp"
#include "person_vla/AppConfig.hpp"
#include "person_vla/IdentityStore.hpp"
#include "person_vla/PersonDetector.hpp"
#include "person_vla/SceneNarrator.hpp"
#include "person_vla/Tracker.hpp"

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <algorithm>
#include <cctype>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <sstream>

namespace {

bool is_integer(const std::string& value) {
    if (value.empty()) {
        return false;
    }
    return std::all_of(value.begin(), value.end(), [](unsigned char ch) { return std::isdigit(ch) != 0; });
}

cv::VideoCapture open_source(const std::string& source) {
    if (is_integer(source)) {
        return cv::VideoCapture(std::stoi(source));
    }

    cv::VideoCapture capture(source, cv::CAP_GSTREAMER);
    if (!capture.isOpened()) {
        capture.open(source);
    }
    return capture;
}

std::string timestamp_string() {
    const auto now = std::chrono::system_clock::now();
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return std::to_string(millis);
}

}  // namespace

int main(int argc, char** argv) {
    person_vla::AppConfig config;
    try {
        config = person_vla::parse_args(argc, argv);
    } catch (const std::exception& error) {
        std::cerr << "[error] " << error.what() << "\n";
        person_vla::print_usage(argv[0]);
        return 2;
    }

    cv::VideoCapture capture = open_source(config.source);
    if (!capture.isOpened()) {
        std::cerr << "[error] Could not open video source: " << config.source << "\n";
        return 1;
    }

    person_vla::PersonDetector detector(config);
    person_vla::Tracker tracker;
    person_vla::IdentityStore identities;
    person_vla::SceneNarrator narrator;
    person_vla::ActionPolicy policy(config.alert_unknown_people);

    std::cout << "[info] detector=" << (detector.using_dnn() ? "OpenCV DNN" : "OpenCV HOG fallback") << "\n";
    std::cout << "[info] keys: q=quit, e=enroll largest person, c=clear profile, s=save frame\n";

    cv::Mat frame;
    while (capture.read(frame)) {
        auto detections = detector.detect(frame);
        auto tracks = tracker.update(detections);

        for (auto& track : tracks) {
            const auto result = identities.identify(frame, track.box);
            track.identity_label = result.first;
            track.identity_score = result.second;
        }

        person_vla::SceneState state;
        state.tracks = tracks;
        state.frame_width = frame.cols;
        state.frame_height = frame.rows;
        state.has_enrolled_target = identities.has_profile();

        const std::string sentence = narrator.describe(state);
        const person_vla::Action action = policy.choose(state);

        for (const auto& track : state.tracks) {
            const cv::Scalar color = track.identity_label == "unknown" ? cv::Scalar(40, 180, 255) : cv::Scalar(80, 220, 80);
            cv::rectangle(frame, track.box, color, 2);

            std::ostringstream label;
            label << "#" << track.id << " " << track.identity_label << " " << static_cast<int>(track.identity_score * 100.0) << "%";
            cv::putText(frame, label.str(), track.box.tl() + cv::Point(0, -6), cv::FONT_HERSHEY_SIMPLEX, 0.55, color, 2);
        }

        cv::putText(frame, sentence, cv::Point(16, 28), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);
        cv::putText(frame, "Action: " + person_vla::to_string(action), cv::Point(16, 58), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);

        cv::imshow("Jetson Person VLA MVP", frame);
        const int key = cv::waitKey(1) & 0xFF;
        if (key == 'q') {
            break;
        }
        if (key == 'c') {
            identities.clear();
            std::cout << "[info] cleared enrolled profile\n";
        }
        if (key == 'e' && !state.tracks.empty()) {
            const auto largest = std::max_element(state.tracks.begin(), state.tracks.end(), [](const auto& a, const auto& b) {
                return a.box.area() < b.box.area();
            });
            identities.enroll("target", frame, largest->box);
            std::cout << "[info] enrolled target from track #" << largest->id << "\n";
        }
        if (key == 's') {
            std::filesystem::create_directories("captures");
            const std::string path = "captures/frame_" + timestamp_string() + ".jpg";
            cv::imwrite(path, frame);
            std::cout << "[info] saved " << path << "\n";
        }
    }

    return 0;
}

