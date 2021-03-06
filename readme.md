# Tetris AI 개발을 위한 c++ 프로젝트

### 예전에 만든 Tetris를 c -> c++ 포팅중
### 매우 간단한 AI완성함
### 할일 목록

1. AI만들기
    1. 다음 블럭을 고려해 더 나은 판단을 하는 AI
        1. 점수체계 정하기
        1. 강화학습
        1. 다음 블럭을 고려해 판단하는 방법 :
            1. 판의 상태를 미래의 가능성에 비례해 점수 매기는 알고리즘
            1. 위 알고리즘을 다음에 나올 블럭들에 대해 재귀적으로 적용
            1. 점수가 낮은 선택들은 가지치기
            1. 가장 점수가 높은 선택대로 결정함
            1. 계산한 결과를 최대한 다음번에도 사용할수 있도록 저장

### 완료된 할일 목록

1. c -> c++ 포팅 완료 하기
    1. 입출력, 블럭상태, 제어를 클래스로 분리
    1. Input, Output 클래스 static 멤버 줄이고 객체만들어 사용하기
    1. 설정값을 저장하는 클래스를 분리
1. AI만들기
    1. 생성장소로부터 가장 먼 곳에 보내는 AI
        1. 블럭이 판에 고정되는 상태들을 저장
        1. 1.에서 저장한 상태들을 생성장소에서 먼 순으로 정렬
        1. 2.에서 정렬한 첫번째 상태로 생성장소부터 도달하는 경로계산
        1. 3.에서 계산한 경로대로 이동하는 일련된 키입력을 생성

### 클래스 목록

1. Point
1. BlockState( -> Point)
1. Input
    1. InputEnum enum
1. Output
1. TetrisSetting
1. Tetris( -> TetrisSetting)
1. TetrisAI

<hr>

### 2020-08-05
1. 완료
    1. TetrisAI 클래스를 연결중 오류 전부 수정
    1. readme.md 에 할일목록 추가

### 2020-08-04
1. 완료
    1. TetrisAI 클래스를 연결중 오류 수정
        1. BlockState 클래스의 구조적인 문제 해결
        1. BlockState를 Point의 자식으로 만듬
1. 진행중
    1. TetrisAI 클래스 연결

### 2020-08-01
1. 완료
    1. Output 클래스에 파일 출력 기능 추가(디버그용)
        1. static std::ofstream Output::consoleOut
1. 진행중
    1. TetrisAI 클래스를 연결중 오류 수정

### 2020-07-28
1. 완료
    1. Main.cpp 의 함수를 모아 Tetris 클래스 만듬(main()은 제외)
    1. Shape 클래스를 BlockState 클래스로 병합
1. 진행중
    1. TetrisAI 클래스와 Tetris 클래스 연결

### 2020-07-27
1. 완료
    1. Output class에 모든 출력관련 코드 옮김
    1. GameInit(), ProcessTimer() 를 정의해 메인함수를 간략화함

### 2020-07-25
1. 완료
    1. 모양마다의 좌표값 4개씩을 Shape 전역변수가 아닌 Shape 클래스로 관리
    1. readme.md 생성
    1. Input class, InputKind enum 정의
    1. Output class 정의