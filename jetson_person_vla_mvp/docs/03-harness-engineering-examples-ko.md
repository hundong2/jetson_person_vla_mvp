# Harness Engineering 예제

## 예제 1: 카메라 없는 로직 테스트

`tests/identity_store_test.cpp`는 빨간 사각형 이미지를 메모리에서 만들고, 이 이미지를 target으로 등록한 뒤 다시 식별합니다. 실제 카메라, 모델, Jetson 없이도 identity 로직의 기준 동작을 확인할 수 있습니다.

## 예제 2: 입력 교체 가능성

같은 실행 파일이 여러 입력을 받습니다.

```bash
./build/person_vla --source 0
./build/person_vla --source ./sample.mp4
./build/person_vla --source rtsp://user:pass@host/stream
./build/person_vla --source "nvarguscamerasrc ! ... ! appsink"
```

## 예제 3: 실패해도 학습이 계속되는 fallback

YOLO 모델 경로가 없으면 HOG detector로 실행됩니다. 정확도는 낮지만 카메라, 빌드, UI, ActionPolicy를 먼저 검증할 수 있습니다.

