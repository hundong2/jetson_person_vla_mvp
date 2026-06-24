#pragma once

#include "person_vla/AppConfig.hpp"
#include "person_vla/Types.hpp"

#include <opencv2/dnn.hpp>
#include <opencv2/objdetect.hpp>

namespace person_vla {

class PersonDetector {
public:
    explicit PersonDetector(const AppConfig& config);

    std::vector<Detection> detect(const cv::Mat& frame);
    bool using_dnn() const { return using_dnn_; }

private:
    std::vector<Detection> detect_with_dnn(const cv::Mat& frame);
    std::vector<Detection> detect_with_hog(const cv::Mat& frame);

    AppConfig config_;
    cv::dnn::Net net_;
    cv::HOGDescriptor hog_;
    std::vector<std::string> class_names_;
    bool using_dnn_ = false;
};

}  // namespace person_vla

