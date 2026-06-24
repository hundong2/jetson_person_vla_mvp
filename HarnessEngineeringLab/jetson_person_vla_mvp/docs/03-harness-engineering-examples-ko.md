# Harness Engineering 예제

## 예제 1: 카메라 없는 로직 테스트

`tests/identity_store_test.cpp`는 빨간 사각형 이미지를 메모리에서 만들고, 이 이미지를 target으로 등록한 뒤 다시 식별합니다. 실제 카메라, 모델, Jetson 없이도 identity 로직의 기준 동작을 확인할 수 있습니다.

명령:

```bash
ctest --test-dir build --output-on-failure
```

## 예제 2: 입력 교체 가능성

같은 실행 파일이 여러 입력을 받습니다.

```bash
./build/person_vla --source 0
./build/person_vla --source ./sample.mp4
./build/person_vla --source rtsp://user:pass@host/stream
./build/person_vla --source "nvarguscamerasrc ! ... ! appsink"
```

이 구조 덕분에 노트북에서 먼저 디버깅하고 Jetson으로 옮길 수 있습니다.

## 예제 3: 실패해도 학습이 계속되는 fallback

YOLO 모델 경로가 없으면 HOG detector로 실행됩니다. 정확도는 낮지만 카메라, 빌드, UI, ActionPolicy를 먼저 검증할 수 있습니다.

## 예제 4: 액션을 별도 계층으로 분리

`ActionPolicy`는 화면 처리나 모델 처리와 분리되어 있습니다. 그래서 나중에 다음 출력으로 바꾸기 쉽습니다.

- 콘솔 로그
- JSON 이벤트
- ROS 2 topic
- Jetson GPIO
- 시리얼 명령

## 예제 5: 실험 로그 설계

다음 단계에서는 매 프레임 또는 1초마다 아래 정보를 JSONL로 저장하면 좋습니다.

```json
{"fps": 18.2, "people": 2, "target_visible": true, "action": "FOLLOW_TARGET", "model": "yolov8n.onnx"}
```

이런 로그가 있어야 모델 교체, 해상도 변경, Jetson 보드 변경의 효과를 비교할 수 있습니다.

