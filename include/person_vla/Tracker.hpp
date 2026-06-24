#pragma once

#include "person_vla/Types.hpp"

#include <vector>

namespace person_vla {

class Tracker {
public:
    std::vector<Track> update(const std::vector<Detection>& detections);

private:
    static double iou(const cv::Rect& a, const cv::Rect& b);

    std::vector<Track> tracks_;
    int next_id_ = 1;
    int max_missed_frames_ = 12;
};

}  // namespace person_vla

