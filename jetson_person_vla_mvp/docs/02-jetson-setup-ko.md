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

