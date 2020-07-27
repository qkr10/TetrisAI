# Tetris AI 개발을 위한 c++ 프로젝트

### 예전에 만든 Tetris를 c -> c++ 포팅중
### 포팅하기 위해 생긴 클래스

1. Point 클래스
1. BlockState 클래스
1. Shape 클래스
1. Input 클래스
    1. InputKind enum 클래스
1. Output 클래스
    1. CURSOR_TYPE enum 클래스

### AI 클래스

1. TetrisAI 클래스

<hr>

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