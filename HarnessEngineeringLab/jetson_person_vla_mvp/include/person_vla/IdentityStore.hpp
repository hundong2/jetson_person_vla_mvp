#pragma once

#include <opencv2/core.hpp>
#include <optional>
#include <string>

namespace person_vla {

class IdentityStore {
public:
    void enroll(const std::string& label, const cv::Mat& frame, const cv::Rect& person_box);
    void clear();
    bool has_profile() const { return profile_.has_value(); }

    std::pair<std::string, double> identify(const cv::Mat& frame, const cv::Rect& person_box) const;

private:
    struct Profile {
        std::string label;
        cv::Mat histogram;
    };

    static cv::Mat build_histogram(const cv::Mat& frame, const cv::Rect& person_box);

    std::optional<Profile> profile_;
};

}  // namespace person_vla

