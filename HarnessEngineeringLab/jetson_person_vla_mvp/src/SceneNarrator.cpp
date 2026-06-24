#include "person_vla/SceneNarrator.hpp"

#include <sstream>

namespace person_vla {

std::string SceneNarrator::describe(const SceneState& state) const {
    std::ostringstream text;

    if (state.tracks.empty()) {
        text << "No person is visible.";
        return text.str();
    }

    text << state.tracks.size() << " person";
    if (state.tracks.size() != 1) {
        text << "s";
    }
    text << " visible";

    bool has_target = false;
    for (const auto& track : state.tracks) {
        if (track.identity_label != "unknown") {
            has_target = true;
            break;
        }
    }

    if (has_target) {
        text << ", including the enrolled target.";
    } else if (state.has_enrolled_target) {
        text << ", but the enrolled target is not confidently identified.";
    } else {
        text << ".";
    }

    return text.str();
}

}  // namespace person_vla

