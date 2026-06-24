#include "person_vla/IdentityStore.hpp"

#include <algorithm>
#include <opencv2/imgproc.hpp>

namespace person_vla {

namespace {

cv::Rect safe_box(const cv::Rect& box, const cv::Size& size) {
    return box & cv::Rect(0, 0, size.width, size.height);
}

}  // namespace

void IdentityStore::enroll(const std::string& label, const cv::Mat& frame, const cv::Rect& person_box) {
    cv::Mat histogram = build_histogram(frame, person_box);
    if (!histogram.empty()) {
        profile_ = Profile{label, histogram};
    }
}

void IdentityStore::clear() {
    profile_.reset();
}

std::pair<std::string, double> IdentityStore::identify(const cv::Mat& frame, const cv::Rect& person_box) const {
    if (!profile_) {
        return {"unknown", 0.0};
    }

    cv::Mat candidate = build_histogram(frame, person_box);
    if (candidate.empty()) {
        return {"unknown", 0.0};
    }

    const double correlation = cv::compareHist(profile_->histogram, candidate, cv::HISTCMP_CORREL);
    const double score = std::max(0.0, std::min(1.0, (correlation + 1.0) / 2.0));
    return score >= 0.78 ? std::make_pair(profile_->label, score) : std::make_pair(std::string("unknown"), score);
}

cv::Mat IdentityStore::build_histogram(const cv::Mat& frame, const cv::Rect& person_box) {
    if (frame.empty()) {
        return {};
    }

    cv::Rect roi_box = safe_box(person_box, frame.size());
    if (roi_box.area() <= 0) {
        return {};
    }

    cv::Mat hsv;
    cv::cvtColor(frame(roi_box), hsv, cv::COLOR_BGR2HSV);

    int channels[] = {0, 1};
    int hist_size[] = {32, 32};
    float hue_range[] = {0.0F, 180.0F};
    float saturation_range[] = {0.0F, 256.0F};
    const float* ranges[] = {hue_range, saturation_range};

    cv::Mat hist;
    cv::calcHist(&hsv, 1, channels, cv::Mat(), hist, 2, hist_size, ranges, true, false);
    cv::normalize(hist, hist, 1.0, 0.0, cv::NORM_L1);
    return hist;
}

}  // namespace person_vla

