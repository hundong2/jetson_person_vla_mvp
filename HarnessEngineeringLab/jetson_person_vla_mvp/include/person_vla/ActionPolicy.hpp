#pragma once

#include "person_vla/Types.hpp"

namespace person_vla {

class ActionPolicy {
public:
    explicit ActionPolicy(bool alert_unknown_people);
    Action choose(const SceneState& state) const;

private:
    bool alert_unknown_people_ = false;
};

}  // namespace person_vla

