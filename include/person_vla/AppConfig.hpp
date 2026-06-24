#pragma once

#include <string>

namespace person_vla {

struct AppConfig {
    std::string source = "0";
    std::string model_path;
    std::string classes_path;
    int input_width = 640;
    int input_height = 640;
    float confidence_threshold = 0.35F;
    float nms_threshold = 0.45F;
    bool prefer_cuda = true;
    bool alert_unknown_people = false;
};

AppConfig parse_args(int argc, char** argv);
void print_usage(const char* program_name);

}  // namespace person_vla

