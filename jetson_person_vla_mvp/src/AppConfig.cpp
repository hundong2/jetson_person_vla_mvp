#include "person_vla/AppConfig.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace person_vla {

void print_usage(const char* program_name) {
    std::cout
        << "Usage: " << program_name << " [options]\n"
        << "  --source <camera-index|video-path|rtsp-url|gstreamer-pipeline>\n"
        << "  --model <onnx-path>        Optional YOLO ONNX model. Falls back to HOG if omitted.\n"
        << "  --classes <names-path>     Optional class names file. COCO person class is assumed.\n"
        << "  --input-width <pixels>     DNN input width, default 640.\n"
        << "  --input-height <pixels>    DNN input height, default 640.\n"
        << "  --conf <0..1>              Detection confidence threshold, default 0.35.\n"
        << "  --nms <0..1>               NMS threshold, default 0.45.\n"
        << "  --cpu                      Disable CUDA backend preference.\n"
        << "  --alert-unknown            Action policy alerts when unknown people appear.\n"
        << "  --help\n";
}

AppConfig parse_args(int argc, char** argv) {
    AppConfig config;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        auto require_value = [&](const std::string& name) -> std::string {
            if (i + 1 >= argc) {
                throw std::runtime_error("Missing value for " + name);
            }
            return argv[++i];
        };

        if (arg == "--source") {
            config.source = require_value(arg);
        } else if (arg == "--model") {
            config.model_path = require_value(arg);
        } else if (arg == "--classes") {
            config.classes_path = require_value(arg);
        } else if (arg == "--input-width") {
            config.input_width = std::stoi(require_value(arg));
        } else if (arg == "--input-height") {
            config.input_height = std::stoi(require_value(arg));
        } else if (arg == "--conf") {
            config.confidence_threshold = std::stof(require_value(arg));
        } else if (arg == "--nms") {
            config.nms_threshold = std::stof(require_value(arg));
        } else if (arg == "--cpu") {
            config.prefer_cuda = false;
        } else if (arg == "--alert-unknown") {
            config.alert_unknown_people = true;
        } else if (arg == "--help") {
            print_usage(argv[0]);
            std::exit(0);
        } else {
            throw std::runtime_error("Unknown argument: " + arg);
        }
    }

    return config;
}

}  // namespace person_vla

