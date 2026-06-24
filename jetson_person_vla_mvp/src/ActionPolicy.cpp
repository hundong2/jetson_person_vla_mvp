#include "person_vla/ActionPolicy.hpp"

namespace person_vla {

std::string to_string(Action action) {
    switch (action) {
        case Action::LogObservation:
            return "LOG_OBSERVATION";
        case Action::FollowTarget:
            return "FOLLOW_TARGET";
        case Action::AlertUnknownPerson:
            return "ALERT_UNKNOWN_PERSON";
        case Action::Idle:
            return "IDLE";
    }
    return "IDLE";
}

ActionPolicy::ActionPolicy(bool alert_unknown_people) : alert_unknown_people_(alert_unknown_people) {}

Action ActionPolicy::choose(const SceneState& state) const {
    if (state.tracks.empty()) {
        return Action::Idle;
    }

    for (const auto& track : state.tracks) {
        if (track.identity_label != "unknown") {
            return Action::FollowTarget;
        }
    }

    if (alert_unknown_people_) {
        return Action::AlertUnknownPerson;
    }

    return Action::LogObservation;
}

}  // namespace person_vla

