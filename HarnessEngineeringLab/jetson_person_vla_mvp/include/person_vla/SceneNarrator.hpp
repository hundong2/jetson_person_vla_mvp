#pragma once

#include "person_vla/Types.hpp"

#include <string>

namespace person_vla {

class SceneNarrator {
public:
    std::string describe(const SceneState& state) const;
};

}  // namespace person_vla

