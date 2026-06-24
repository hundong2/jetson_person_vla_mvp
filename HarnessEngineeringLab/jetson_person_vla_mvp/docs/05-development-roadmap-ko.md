# 개발 로드맵

## Phase 1: 현재 MVP

- OpenCV VideoCapture
- HOG fallback
- YOLO ONNX person detection
- IoU tracking
- 단일 appearance profile enrollment
- 규칙 기반 scene narration/action

## Phase 2: Jetson 최적화

- 모델 입력 크기별 FPS 측정
- TensorRT 엔진 변환
- GStreamer pipeline 고정
- JSONL 성능 로그 추가
- headless 실행 모드 추가

## Phase 3: 더 강한 사람 식별

- DeepSORT 또는 ByteTrack 도입
- OSNet 같은 person re-identification embedding 검토
- 장면별 threshold calibration
- 등록/삭제 UI 또는 CLI 명령 추가

## Phase 4: VLA 확장

- SceneState를 JSON schema로 정의
- LLM에 넘길 요약 프롬프트 추가
- ActionPolicy를 ROS 2 command publisher로 교체
- 사람이 승인한 action만 실제 장치로 출력

## Phase 5: 운영 품질

- systemd service
- watchdog
- structured logs
- health endpoint
- 모델/설정 버전 기록
- 개인정보 삭제 명령

