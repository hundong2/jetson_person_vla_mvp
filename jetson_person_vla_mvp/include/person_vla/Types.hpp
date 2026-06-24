#pragma once

#include <opencv2/core.hpp>
#include <string>
#include <vector>

namespace person_vla {

struct Detection {
    cv::Rect box;
    float confidence = 0.0F;
    int class_id = -1;
    std::string class_name;
};

struct Track {
    int id = -1;
    cv::Rect box;
    int missed_frames = 0;
    float confidence = 0.0F;
    std::string identity_label = "unknown";
    double identity_score = 0.0;
};

struct SceneState {
    std::vector<Track> tracks;
    int frame_width = 0;
    int frame_height = 0;
    bool has_enrolled_target = false;
};

enum class Action {
    LogObservation,
    FollowTarget,
    AlertUnknownPerson,
    Idle
};

std::string to_string(Action action);

}  // namespace person_vla

