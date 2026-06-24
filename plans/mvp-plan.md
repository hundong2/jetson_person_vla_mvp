# MVP Plan: Jetson Person VLA

## Goal

Build a C++ OpenCV project that runs on desktop Linux, Jetson Nano, and Jetson Orin to detect people in video, assign short-lived track IDs, enroll one consenting target by appearance, produce a scene sentence, and choose a simple robot-style action.

## Acceptance Criteria

- [x] CMake project builds a runnable `person_vla` executable.
- [x] Video source can be a camera index, video path, RTSP URL, or Jetson GStreamer pipeline.
- [x] Person detection supports OpenCV DNN YOLO ONNX and a HOG fallback.
- [x] Tracking logic produces stable temporary IDs across nearby frames.
- [x] Identity MVP can enroll and match one local appearance profile.
- [x] VLA loop produces visual overlays, a language sentence, and an action string.
- [x] At least one camera-free test validates identity logic.
- [x] Korean learning guide explains the harness engineering workflow.

## Harness Checks

- Build check: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j$(nproc)`
- Logic check: `ctest --test-dir build --output-on-failure`
- Runtime smoke check: `./build/person_vla --source 0`
- Jetson CSI check: `./scripts/run_jetson_csi.sh`

