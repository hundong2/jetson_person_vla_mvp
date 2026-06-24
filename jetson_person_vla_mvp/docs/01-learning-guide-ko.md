# 학습 가이드: C++ Harness Engineering + Jetson Person VLA

## 1. 이 프로젝트의 MVP

목표는 거대한 자율주행/로봇 시스템이 아니라, 검증 가능한 작은 VLA 루프를 만드는 것입니다.

- Visual: 카메라 프레임을 읽고 사람을 찾습니다.
- Language: 현재 장면을 짧은 문장으로 요약합니다.
- Action: 장면 상태에 따라 다음 행동 문자열을 선택합니다.

`VLA`라는 이름을 쓰지만, 이 MVP는 대형 언어모델을 필수로 쓰지 않습니다. 먼저 C++/OpenCV/Jetson에서 안정적으로 돌아가는 감지, 추적, 설명, 액션 선택 루프를 만든 뒤 LLM 또는 로봇 제어를 붙이는 순서가 더 좋습니다.

## 2. Harness Engineering 관점

좋은 harness는 실제 하드웨어가 없어도 핵심 로직을 검증할 수 있게 만듭니다.

- 카메라 입력은 `--source`로 주입합니다.
- 모델 경로는 `--model`로 주입합니다.
- 카메라 없이 검증 가능한 로직은 `tests/`에 둡니다.
- 실시간 화면에는 감지 박스, ID, 설명, 액션을 표시합니다.
- 실패 가능성이 큰 부분은 HOG fallback처럼 학습용 대체 경로를 둡니다.

## 3. 권장 학습 순서

1. `README.md`의 HOG fallback으로 먼저 실행합니다.
2. `tests/identity_store_test.cpp`를 읽고 카메라 없는 테스트가 어떤 가치가 있는지 확인합니다.
3. `src/PersonDetector.cpp`에서 YOLO ONNX 출력 파싱을 읽습니다.
4. `src/Tracker.cpp`에서 IoU 기반 간단 추적을 읽습니다.
5. `src/IdentityStore.cpp`에서 색상 히스토그램 기반 appearance matching을 읽습니다.
6. `src/SceneNarrator.cpp`, `src/ActionPolicy.cpp`에서 VLA의 Language/Action 계층을 확인합니다.
7. Jetson에서 USB 카메라, CSI 카메라 순서로 검증합니다.

## 4. 중요한 한계

현재 identity는 얼굴 인식이나 법적 신원 확인이 아닙니다. 색상/외형 기반의 학습용 임시 식별입니다. 조명, 옷 색, 자세, 가림에 취약합니다.

사람을 식별하는 시스템은 반드시 동의, 고지, 보관 기간, 접근 권한, 삭제 절차를 함께 설계해야 합니다.

