#include "person_vla/Tracker.hpp"

#include <algorithm>

namespace person_vla {

double Tracker::iou(const cv::Rect& a, const cv::Rect& b) {
    const cv::Rect intersection = a & b;
    const double intersection_area = static_cast<double>(intersection.area());
    const double union_area = static_cast<double>(a.area() + b.area()) - intersection_area;
    return union_area <= 0.0 ? 0.0 : intersection_area / union_area;
}

std::vector<Track> Tracker::update(const std::vector<Detection>& detections) {
    std::vector<bool> matched_detections(detections.size(), false);

    for (auto& track : tracks_) {
        int best_index = -1;
        double best_iou = 0.0;

        for (std::size_t i = 0; i < detections.size(); ++i) {
            if (matched_detections[i]) {
                continue;
            }
            const double score = iou(track.box, detections[i].box);
            if (score > best_iou) {
                best_iou = score;
                best_index = static_cast<int>(i);
            }
        }

        if (best_index >= 0 && best_iou >= 0.25) {
            const auto& detection = detections[best_index];
            track.box = detection.box;
            track.confidence = detection.confidence;
            track.missed_frames = 0;
            matched_detections[best_index] = true;
        } else {
            track.missed_frames += 1;
        }
    }

    for (std::size_t i = 0; i < detections.size(); ++i) {
        if (!matched_detections[i]) {
            Track track;
            track.id = next_id_++;
            track.box = detections[i].box;
            track.confidence = detections[i].confidence;
            tracks_.push_back(track);
        }
    }

    tracks_.erase(
        std::remove_if(tracks_.begin(), tracks_.end(), [&](const Track& track) {
            return track.missed_frames > max_missed_frames_;
        }),
        tracks_.end()
    );

    return tracks_;
}

}  // namespace person_vla

