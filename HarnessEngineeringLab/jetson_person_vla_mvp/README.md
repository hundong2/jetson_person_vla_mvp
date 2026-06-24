# Jetson Person VLA MVP

C++ harness engineering practice project for OpenCV on Jetson Nano and Jetson Orin.

The MVP is a small Visual-Language-Action style application:

1. Visual: read frames from a webcam, CSI camera, video file, or RTSP stream.
2. Perception: detect people with OpenCV DNN YOLO, or fall back to OpenCV HOG for a no-model smoke test.
3. Identity: assign temporary track IDs and optionally enroll a consenting person's appearance as a local profile.
4. Language: generate a short Korean/English scene sentence.
5. Action: choose a simple action such as `FOLLOW_TARGET`, `LOG_OBSERVATION`, or `ALERT_UNKNOWN_PERSON`.

This is not face recognition. The identity MVP uses simple appearance histograms and session tracking, so it is intended for learning, lab demos, and consenting subjects only. For production identity, add explicit consent, retention policy, encryption, access control, and legal review.

## Project Layout

```text
jetson_person_vla_mvp/
  CMakeLists.txt
  config/person_vla.yaml
  docs/
    01-learning-guide-ko.md
    02-jetson-setup-ko.md
    03-harness-engineering-examples-ko.md
    04-safety-and-privacy-ko.md
    05-development-roadmap-ko.md
  include/person_vla/
  plans/mvp-plan.md
  todo.md
  scripts/
    run_jetson_csi.sh
    run_usb_camera.sh
  src/
  tests/
```

## Quick Start on Ubuntu or Jetson

Install dependencies:

```bash
sudo apt update
sudo apt install -y build-essential cmake pkg-config libopencv-dev
```

Build:

```bash
cd HarnessEngineeringLab/jetson_person_vla_mvp
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

Run with webcam and HOG fallback:

```bash
./build/person_vla --source 0
```

Run with YOLO ONNX:

```bash
./build/person_vla --source 0 --model ./models/yolov8n.onnx --classes ./models/coco.names
```

Run the small harness test:

```bash
ctest --test-dir build --output-on-failure
```

## Controls

- `q`: quit
- `e`: enroll the largest visible person as the target profile
- `c`: clear enrolled profile
- `s`: save the current annotated frame to `captures/`

## Recommended Model

Use a COCO-trained YOLO ONNX model such as `yolov8n.onnx`. The code filters to class `person`.

For Jetson Orin, use TensorRT later for higher FPS. For Nano, start with `320x320` input or HOG fallback to validate camera, harness, and control flow before optimizing.

## Learning Path

Read these in order:

1. `docs/01-learning-guide-ko.md`
2. `docs/02-jetson-setup-ko.md`
3. `docs/03-harness-engineering-examples-ko.md`
4. `docs/04-safety-and-privacy-ko.md`
5. `docs/05-development-roadmap-ko.md`
6. `todo.md`

## Harness Engineering Goal

The project demonstrates a practical harness loop:

1. Fix the MVP goal and acceptance criteria in `plans/mvp-plan.md`.
2. Keep camera/model/hardware dependencies injectable through CLI and config.
3. Make risky runtime behavior observable through logs and on-frame overlays.
4. Add test seams for logic that can be validated without a camera.
5. Verify on desktop first, then Jetson USB camera, then Jetson CSI camera.
