# TODO: Expert Expansion Study Plan

이 문서는 `Jetson Person VLA MVP`를 더 강한 영상 인식, 사람 재식별, VLA/로봇 액션 시스템으로 확장하기 위한 학습 TODO입니다.

## 0. 현재 MVP 기준선

- [ ] HOG fallback으로 USB 카메라 실행 확인
- [ ] YOLO ONNX 모델로 사람 검출 실행 확인
- [ ] `e` 키로 target appearance profile 등록 후 재식별 동작 확인
- [ ] Jetson Nano와 Orin에서 FPS, 지연 시간, CPU/GPU 사용률 기록
- [ ] 오탐/미탐 사례를 `captures/`에 저장하고 원인 기록

## 1. Object Detection 역사와 기초

### 큰 흐름

- 2001 전후: HOG, Haar cascade 같은 수작업 특징 기반 검출이 실시간 비전의 출발점이었습니다.
- 2012 이후: AlexNet 이후 CNN 특징이 기존 특징 공학을 빠르게 대체했습니다.
- 2014-2015: R-CNN, Fast R-CNN, Faster R-CNN 계열이 region proposal 기반 검출을 정교하게 만들었습니다.
- 2015-2016: YOLO, SSD가 one-stage detector 흐름을 만들며 실시간 검출의 기준을 바꿨습니다.
- 2020년대: YOLO 계열, transformer detector, NAS 기반 경량 detector가 edge device 배포의 중심이 되었습니다.

### 읽을 논문

- [ ] Faster R-CNN: Towards Real-Time Object Detection with Region Proposal Networks, 2015
- [ ] You Only Look Once: Unified, Real-Time Object Detection, 2015/2016
- [ ] SSD: Single Shot MultiBox Detector, 2015/2016

### 실습 TODO

- [ ] `--input-width 320/416/640` 별 FPS와 검출률 비교
- [ ] 사람 한 명, 여러 명, 역광, 가림, 원거리 영상을 각각 저장해 테스트 세트 만들기
- [ ] confidence threshold와 NMS threshold를 바꿔 오탐/미탐 변화 기록

## 2. Multi-Object Tracking 역사와 기초

- [ ] SORT: Simple Online and Realtime Tracking, 2016
- [ ] DeepSORT: Simple Online and Realtime Tracking with a Deep Association Metric, 2017
- [ ] ByteTrack: Multi-Object Tracking by Associating Every Detection Box, 2021/2022

실습:

- [ ] `Tracker.cpp`에 Kalman filter 예측 단계 추가
- [ ] IoU matching을 Hungarian algorithm으로 교체
- [ ] low-score detection을 별도로 받아 ByteTrack 방식 2단계 matching 실험
- [ ] ID switch, track lost count, average track length를 로그로 남기기

## 3. Person Re-Identification 역사와 기초

- [ ] Omni-Scale Feature Learning for Person Re-Identification, ICCV 2019
- [ ] 현재 histogram matching 실패 사례 수집
- [ ] person crop 저장 옵션 추가
- [ ] OSNet 또는 MobileNet 기반 ReID embedding을 ONNX로 export
- [ ] cosine similarity 기반 `IdentityStore` 구현 추가
- [ ] threshold calibration test 추가

## 4. Jetson Edge AI 최적화

- [ ] NVIDIA TensorRT Documentation 읽기
- [ ] NVIDIA DeepStream SDK Developer Guide 읽기
- [ ] OpenCV DNN CPU, OpenCV DNN CUDA, TensorRT engine FPS 비교
- [ ] Jetson Nano에서 320 입력, Orin에서 640 입력 benchmark
- [ ] `--headless` 모드 추가
- [ ] JSONL 로그 추가: fps, people count, target visible, action, model, input size

## 5. VLA와 로봇 Action 확장

- [ ] `SceneState`를 JSON schema로 정의
- [ ] `SceneNarrator`에 한국어 출력 옵션 추가
- [ ] `ActionPolicy`를 plugin 구조로 분리
- [ ] `FOLLOW_TARGET`, `STOP`, `SEARCH_TARGET`, `ALERT` action enum 확장
- [ ] ROS 2 topic publisher 예제 추가
- [ ] 위험 action에는 confirmation gate 추가

## 6. Source Links

- YOLO: https://arxiv.org/abs/1506.02640
- Faster R-CNN: https://arxiv.org/abs/1506.01497
- SSD: https://arxiv.org/abs/1512.02325
- SORT: https://arxiv.org/abs/1602.00763
- DeepSORT: https://arxiv.org/abs/1703.07402
- ByteTrack: https://arxiv.org/abs/2110.06864
- OSNet: https://openaccess.thecvf.com/content_ICCV_2019/papers/Zhou_Omni-Scale_Feature_Learning_for_Person_Re-Identification_ICCV_2019_paper.pdf
- NVIDIA TensorRT: https://docs.nvidia.com/deeplearning/tensorrt/latest/index.html
- NVIDIA DeepStream: https://docs.nvidia.com/metropolis/deepstream/dev-guide/

