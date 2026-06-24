# TODO: Expert Expansion Study Plan

이 문서는 `Jetson Person VLA MVP`를 더 강한 영상 인식, 사람 재식별, VLA/로봇 액션 시스템으로 확장하기 위한 학습 TODO입니다. 단순 구현 목록이 아니라 기술의 역사, 핵심 논문, 읽는 순서, 실습 과제를 함께 정리합니다.

## 0. 현재 MVP 기준선

- [ ] HOG fallback으로 USB 카메라 실행 확인
- [ ] YOLO ONNX 모델로 사람 검출 실행 확인
- [ ] `e` 키로 target appearance profile 등록 후 재식별 동작 확인
- [ ] Jetson Nano와 Orin에서 FPS, 지연 시간, CPU/GPU 사용률 기록
- [ ] 오탐/미탐 사례를 `captures/`에 저장하고 원인 기록

기준선 없이 모델을 바꾸면 개선인지 착시인지 알기 어렵습니다. 먼저 현재 MVP의 한계를 숫자와 예시 이미지로 고정하세요.

## 1. Object Detection 역사와 기초

### 큰 흐름

- 2001 전후: HOG, Haar cascade 같은 수작업 특징 기반 검출이 실시간 비전의 출발점이었습니다.
- 2012 이후: AlexNet 이후 CNN 특징이 기존 특징 공학을 빠르게 대체했습니다.
- 2014-2015: R-CNN, Fast R-CNN, Faster R-CNN 계열이 region proposal 기반 검출을 정교하게 만들었습니다.
- 2015-2016: YOLO, SSD가 one-stage detector 흐름을 만들며 실시간 검출의 기준을 바꿨습니다.
- 2020년대: YOLO 계열, transformer detector, NAS 기반 경량 detector가 edge device 배포의 중심이 되었습니다.

### 읽을 논문

- [ ] Faster R-CNN: Towards Real-Time Object Detection with Region Proposal Networks, 2015
  - 핵심: Region Proposal Network가 feature map을 공유해 proposal 생성 병목을 줄였습니다.
  - 배울 점: two-stage detector가 왜 정확도에 강하고, edge 실시간 처리에서는 왜 부담이 되는지 이해합니다.
  - MVP 연결: Jetson에서는 정확도와 지연 시간 사이의 trade-off를 판단하는 기준이 됩니다.

- [ ] You Only Look Once: Unified, Real-Time Object Detection, 2015/2016
  - 핵심: 검출을 bounding box와 class probability를 한 번에 예측하는 regression 문제로 재정의했습니다.
  - 배울 점: single network, end-to-end, real-time detector라는 사고방식입니다.
  - MVP 연결: 현재 `PersonDetector.cpp`의 YOLO ONNX 경로가 이 계열입니다.

- [ ] SSD: Single Shot MultiBox Detector, 2015/2016
  - 핵심: 여러 feature map 위치에서 default box를 예측해 다양한 크기의 물체를 처리합니다.
  - 배울 점: multi-scale feature와 anchor/default box 개념입니다.
  - MVP 연결: 작은 사람, 먼 사람, 해상도 변화에서 왜 검출 성능이 흔들리는지 설명할 수 있습니다.

### 실습 TODO

- [ ] `--input-width 320/416/640` 별 FPS와 검출률 비교
- [ ] 사람 한 명, 여러 명, 역광, 가림, 원거리 영상을 각각 저장해 테스트 세트 만들기
- [ ] YOLOv5/v8 ONNX 출력 shape를 로그로 출력하는 debug mode 추가
- [ ] confidence threshold와 NMS threshold를 바꿔 오탐/미탐 변화 기록

## 2. Multi-Object Tracking 역사와 기초

### 큰 흐름

- 고전 추적: Kalman filter, optical flow, mean-shift처럼 motion 중심으로 움직임을 추정했습니다.
- Tracking-by-detection: 매 프레임 detector가 낸 box를 시간축으로 연결하는 방식이 주류가 되었습니다.
- SORT: Kalman filter와 Hungarian matching만으로 단순하고 빠른 baseline을 제시했습니다.
- DeepSORT: appearance embedding을 추가해 occlusion 이후 ID switch를 줄였습니다.
- ByteTrack: 낮은 confidence box까지 association에 활용해 track fragmentation을 줄였습니다.

### 읽을 논문

- [ ] Simple Online and Realtime Tracking, 2016
  - 핵심: 좋은 detector와 단순한 Kalman + Hungarian association만으로도 강한 온라인 tracking baseline을 만들 수 있습니다.
  - 배울 점: tracking 품질의 상당 부분은 detector 품질에 의해 결정됩니다.
  - MVP 연결: 현재 `Tracker.cpp`는 IoU만 쓰는 가장 작은 tracking-by-detection 예제입니다.

- [ ] Simple Online and Realtime Tracking with a Deep Association Metric, 2017
  - 핵심: SORT에 appearance metric을 추가해 긴 occlusion 상황의 ID switch를 줄였습니다.
  - 배울 점: motion cue와 appearance cue를 같이 쓰는 이유입니다.
  - MVP 연결: 현재 histogram identity를 deep embedding으로 대체할 때의 중간 다리입니다.

- [ ] ByteTrack: Multi-Object Tracking by Associating Every Detection Box, 2021/2022
  - 핵심: 높은 score box만 쓰면 가려진 사람의 track이 끊기므로 낮은 score detection도 tracklet과 비교해 살립니다.
  - 배울 점: confidence threshold는 detection 문제이면서 동시에 tracking 문제입니다.
  - MVP 연결: Jetson에서 사람 가림이 잦다면 ByteTrack 계열이 가장 실용적인 다음 단계입니다.

### 실습 TODO

- [ ] `Tracker.cpp`에 Kalman filter 예측 단계 추가
- [ ] IoU matching을 Hungarian algorithm으로 교체
- [ ] low-score detection을 별도로 받아 ByteTrack 방식 2단계 matching 실험
- [ ] ID switch, track lost count, average track length를 로그로 남기기

## 3. Person Re-Identification 역사와 기초

### 큰 흐름

- 초기 ReID: 색상 히스토그램, LBP, HOG 등 hand-crafted feature가 중심이었습니다.
- CNN ReID: 사람 crop을 embedding vector로 바꾸고 metric learning/triplet loss로 비교했습니다.
- Part-based ReID: 사람 몸을 stripe/part로 나눠 자세 변화에 대응했습니다.
- Omni-scale ReID: 여러 receptive field scale을 동시에 학습해 local/global appearance를 함께 봅니다.
- 실전 ReID: 개인정보, 동의, 데이터 보관 정책이 모델 성능만큼 중요합니다.

### 읽을 논문

- [ ] Omni-Scale Feature Learning for Person Re-Identification, ICCV 2019
  - 핵심: OSNet은 여러 convolution stream으로 다양한 scale의 feature를 만들고, input-dependent gate로 동적으로 합칩니다.
  - 배울 점: 한 사람의 옷, 가방, 체형, 부분 패턴이 서로 다른 scale에서 의미를 가진다는 점입니다.
  - MVP 연결: 현재 `IdentityStore.cpp`의 HSV histogram은 OSNet 같은 embedding 모델로 대체할 수 있습니다.

### 실습 TODO

- [ ] 현재 histogram matching 실패 사례 수집
- [ ] person crop 저장 옵션 추가
- [ ] OSNet 또는 MobileNet 기반 ReID embedding을 ONNX로 export
- [ ] cosine similarity 기반 `IdentityStore` 구현 추가
- [ ] threshold calibration notebook 또는 C++ test 추가
- [ ] profile 자동 만료와 삭제 명령 추가

## 4. Jetson Edge AI 최적화

### 큰 흐름

- OpenCV DNN: 시작이 쉽고 C++ 통합이 편합니다.
- TensorRT: NVIDIA GPU에서 inference graph 최적화, precision 최적화, layer fusion 등을 수행합니다.
- DeepStream: GStreamer 기반으로 decode, preprocess, inference, tracking, metadata pipeline을 최적화합니다.
- ROS 2/robot stack: 인식 결과를 실제 로봇 action으로 연결할 때 필요합니다.

### 읽을 문서

- [ ] NVIDIA TensorRT Documentation
  - 핵심: ONNX/PyTorch/TensorFlow 모델을 NVIDIA GPU inference용으로 최적화합니다.
  - 배울 점: FP16/INT8, dynamic shape, engine build, runtime context 개념입니다.
  - MVP 연결: Orin에서 YOLO FPS를 올리는 핵심 경로입니다.

- [ ] NVIDIA DeepStream SDK Developer Guide
  - 핵심: USB/CSI/RTSP/file video stream을 받아 AI video analytics pipeline을 구성합니다.
  - 배울 점: GStreamer pipeline, plugin, metadata, multi-stream 구조입니다.
  - MVP 연결: 카메라가 여러 대가 되거나 FPS가 중요해지면 OpenCV 루프보다 DeepStream이 유리합니다.

### 실습 TODO

- [ ] OpenCV DNN CPU, OpenCV DNN CUDA, TensorRT engine FPS 비교
- [ ] Jetson Nano에서 320 입력, Orin에서 640 입력 benchmark
- [ ] `--headless` 모드 추가
- [ ] JSONL 로그 추가: fps, people count, target visible, action, model, input size
- [ ] GStreamer CSI pipeline을 config 파일로 분리
- [ ] DeepStream prototype 브랜치 만들기

## 5. VLA와 로봇 Action 확장

### 큰 흐름

- Perception-only: bounding box, class, confidence를 출력합니다.
- Scene graph/state: 객체, 위치, 관계, 시간 상태를 구조화합니다.
- Language: 장면 상태를 사람이 읽는 문장 또는 LLM prompt로 변환합니다.
- Action: 규칙, planner, policy model, ROS 2 command로 실제 행동을 선택합니다.
- Human-in-the-loop: 사람에게 영향을 주는 action은 승인 단계를 둡니다.

### 실습 TODO

- [ ] `SceneState`를 JSON schema로 정의
- [ ] `SceneNarrator`에 한국어 출력 옵션 추가
- [ ] `ActionPolicy`를 plugin 구조로 분리
- [ ] `FOLLOW_TARGET`, `STOP`, `SEARCH_TARGET`, `ALERT` action enum 확장
- [ ] ROS 2 topic publisher 예제 추가
- [ ] 위험 action에는 confirmation gate 추가

## 6. 안전, 개인정보, 운영 기준

### 필수 TODO

- [ ] 등록 전 동의 문구 표시
- [ ] profile 저장 여부 기본값 off 유지
- [ ] 저장된 profile 삭제 명령 구현
- [ ] 원본 영상 저장 금지 옵션 기본값 유지
- [ ] 로그에 얼굴 이미지나 원본 crop을 남기지 않는 모드 추가
- [ ] 오탐 시 사람이 action을 취소할 수 있는 UI/입력 추가

### 검토 질문

- 이 시스템은 어디에서 촬영하는가?
- 촬영 대상은 고지와 동의를 받았는가?
- 식별 결과가 사람에게 어떤 영향을 주는가?
- 영상 또는 profile은 얼마나 오래 보관하는가?
- 누가 삭제할 수 있는가?
- 네트워크로 전송되는 데이터가 있는가?

## 7. 추천 읽기 순서

1. YOLO paper
2. SSD paper
3. Faster R-CNN paper
4. SORT paper
5. DeepSORT paper
6. ByteTrack paper
7. OSNet paper
8. TensorRT documentation
9. DeepStream documentation

이 순서가 좋은 이유는 detection, tracking, identity, deployment, action system으로 사고를 넓혀가기 때문입니다.

## 8. Source Links

- YOLO: https://arxiv.org/abs/1506.02640
- Faster R-CNN: https://arxiv.org/abs/1506.01497
- SSD: https://arxiv.org/abs/1512.02325
- SORT: https://arxiv.org/abs/1602.00763
- DeepSORT: https://arxiv.org/abs/1703.07402
- ByteTrack: https://arxiv.org/abs/2110.06864
- OSNet: https://openaccess.thecvf.com/content_ICCV_2019/papers/Zhou_Omni-Scale_Feature_Learning_for_Person_Re-Identification_ICCV_2019_paper.pdf
- NVIDIA TensorRT: https://docs.nvidia.com/deeplearning/tensorrt/latest/index.html
- NVIDIA DeepStream: https://docs.nvidia.com/metropolis/deepstream/dev-guide/

