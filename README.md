# OpenGL Project

OpenGL 프로젝트 <현재 진행중>. GLFW 윈도우 생성, 셰이더/프로그램 추상화, 버텍스 버퍼, 텍스처, 카메라, ImGui 통합까지 구현되어 있다.

## 요구 사항

- CMake 4.0 이상
- C++20 지원 컴파일러 (MSVC / GCC / Clang)
- OpenGL 3.3 이상 지원 GPU

## 의존성

CMake FetchContent로 빌드 시 자동 다운로드된다. (`cmake/dependencies/`)

| 라이브러리 | 용도 |
| --- | --- |
| GLFW | 윈도우 및 입력 처리 |
| GLAD | OpenGL 함수 로더 (`external/glad`) |
| GLM | 수학 라이브러리 |
| spdlog | 로깅 |
| stb_image | 이미지 로딩 |
| Dear ImGui | GUI (`imgui/`, 소스 포함) |

## 빌드

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

실행 파일: `build/OpenGL_Project` (Windows: `OpenGL_Project.exe`)

## 디렉터리 구조

```
├── CMakeLists.txt
├── cmake/                  # 의존성 CMake 모듈
│   └── dependencies/
├── external/glad/          # GLAD 소스
├── imgui/                  # Dear ImGui 소스
├── shader/                 # GLSL 셰이더 (.vert / .frag)
└── src/
    ├── main.cpp            # 엔트리 포인트
    ├── common.*            # 공통 유틸 (파일 로딩 등)
    ├── context.*           # 렌더링 컨텍스트
    ├── shader.*            # 셰이더 래퍼
    ├── program.*           # 셰이더 프로그램 래퍼
    ├── buffer.*            # VBO/EBO 래퍼
    ├── vertex_layout.*     # VAO 래퍼
    ├── image.*             # 이미지 로딩 (stb_image)
    └── texture.*           # 텍스처 래퍼
```

## 설정

윈도우 이름 및 크기는 `CMakeLists.txt`에서 정의한다.

```cmake
set(WINDOW_NAME "SeongcheolJeon's OpenGL")
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 540)
```