# Jetson Nano / Orin 설정 가이드

## 1. 기본 패키지

```bash
sudo apt update
sudo apt install -y build-essential cmake pkg-config git libopencv-dev v4l-utils
```

카메라 확인:

```bash
v4l2-ctl --list-devices
```

## 2. USB 카메라 실행

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
./scripts/run_usb_camera.sh
```

## 3. CSI 카메라 실행

```bash
./scripts/run_jetson_csi.sh
```

CSI 카메라가 안 열리면 먼저 아래 명령으로 Jetson 카메라 스택을 확인합니다.

```bash
gst-launch-1.0 nvarguscamerasrc ! nvoverlaysink
```

## 4. Jetson 성능 팁

Nano:

- `--input-width 320 --input-height 320`부터 시작합니다.
- HOG fallback으로 카메라와 UI를 먼저 검증합니다.
- 전원 어댑터와 냉각 상태를 확인합니다.

Orin:

- `--model yolov8n.onnx`로 시작한 뒤 TensorRT 변환을 검토합니다.
- OpenCV DNN CUDA 지원 여부는 설치된 OpenCV 빌드에 따라 달라집니다.
- 실전에서는 DeepStream 또는 TensorRT C++ API가 더 높은 FPS를 냅니다.

## 5. 모델 파일

모델 파일은 저장소에 포함하지 않습니다. 예시는 다음 구조를 권장합니다.

```text
models/
  yolov8n.onnx
  coco.names
```

`coco.names`의 첫 줄은 `person`이어야 합니다.

