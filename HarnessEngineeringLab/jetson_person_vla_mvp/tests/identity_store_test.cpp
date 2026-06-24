#include "person_vla/IdentityStore.hpp"

#include <cassert>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat frame(160, 160, CV_8UC3, cv::Scalar(30, 30, 30));
    cv::rectangle(frame, cv::Rect(40, 20, 60, 120), cv::Scalar(0, 0, 255), cv::FILLED);

    person_vla::IdentityStore store;
    store.enroll("target", frame, cv::Rect(40, 20, 60, 120));

    const auto same = store.identify(frame, cv::Rect(40, 20, 60, 120));
    assert(same.first == "target");
    assert(same.second > 0.95);

    cv::Mat different(160, 160, CV_8UC3, cv::Scalar(30, 30, 30));
    cv::rectangle(different, cv::Rect(40, 20, 60, 120), cv::Scalar(255, 0, 0), cv::FILLED);
    const auto other = store.identify(different, cv::Rect(40, 20, 60, 120));
    assert(other.first == "unknown");

    return 0;
}

