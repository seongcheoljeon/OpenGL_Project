# OpenGL Study Project

OpenGL 렌더링 기법을 단계적으로 학습하고 구현하는 C++ 프로젝트입니다.  
Texture Mapping부터 시작하여 Normal Mapping, Shadow Mapping, Deferred Shading, SSAO, PBR, IBL, BVH 등 렌더링 핵심 기법들을 점진적으로 확장할 예정입니다.

---

## 개발 환경

| 항목 | 내용 |
|---|---|
| Language | C++20 |
| Build System | CMake 4.0+ |
| Compiler | GCC / Clang / MSVC |
| Window | 960 x 540 |

---

## 의존성

모든 의존성은 CMake `FetchContent`로 자동 관리됩니다. 별도 설치 불필요.

| 라이브러리 | 버전 | 용도 |
|---|---|---|
| [GLFW](https://github.com/glfw/glfw) | 3.3.10 | 윈도우 생성 및 입력 처리 |
| [GLAD](https://glad.dav1d.de/) | - | OpenGL 함수 로딩 |
| [spdlog](https://github.com/gabime/spdlog) | v1.x | 로깅 |
| [stb_image](https://github.com/nothings/stb) | master | 이미지 로딩 |
| [glm](https://github.com/g-truc/glm) | 1.0.1 | 수학 라이브러리 (벡터, 행렬) |

---

## 빌드 방법

```bash
git clone https://github.com/your-repo/OpenGL_Project.git
cd OpenGL_Project

cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-debug
```

---

## 프로젝트 구조

```
OpenGL_Project/
├── cmake/
│   ├── Dependency.cmake          # 의존성 진입점
│   └── dependencies/
│       ├── glad.cmake
│       ├── glfw.cmake
│       ├── glm.cmake
│       ├── spdlog.cmake
│       └── stb_image.cmake
├── external/
│   ├── glad/                     # GLAD 소스
│   └── include/
│       ├── stb/stb_image.h       # stb_image 헤더 (복사본)
│       └── glm/                  # glm 헤더 (복사본)
├── shader/
│   ├── simple.vert
│   └── simple.frag
├── image/                        # 텍스처 리소스
├── src/
│   ├── main.cpp
│   ├── common.h                  # 공통 헤더 및 매크로
│   ├── context.h / context.cpp   # 렌더 컨텍스트
│   ├── shader.h / shader.cpp     # 셰이더 컴파일
│   ├── program.h / program.cpp   # 셰이더 프로그램 링크
│   ├── buffer.h / buffer.cpp     # VBO / EBO
│   ├── vertex_layout.h / .cpp    # VAO
│   ├── image.h / image.cpp       # 이미지 로딩 (stb_image 래퍼)
│   └── texture.h / texture.cpp   # OpenGL 텍스처
└── CMakeLists.txt
```

---

## 구현 현황 및 로드맵

### ✅ 완료

- **기반 시스템** — GLFW 윈도우, OpenGL Context, GLAD 초기화
- **셰이더 시스템** — GLSL 파일 로딩, 컴파일, 링크, 에러 리포팅
- **버퍼 시스템** — VBO, EBO, VAO 추상화
- **텍스처 시스템** — 이미지 로딩, 멀티 텍스처, Mipmap, Wrap/Filter 설정

### 🔲 예정

구현 순서는 아래 의존성 흐름을 따릅니다.

- **Lighting** — Phong / Blinn-Phong
- **Normal Mapping** — Tangent space 기반 법선 변환, PBR 이전 필수 단계
- **Shadow Mapping** — Depth Map 기반 그림자
- **Cascaded Shadow Maps (CSM)** — Shadow Mapping 원거리 품질 개선
- **Deferred Shading** — G-Buffer 기반 다중 광원
- **SSAO** — Screen Space Ambient Occlusion
- **Screen Space Reflections (SSR)** — Deferred Shading 기반 반사
- **HDR + Tone Mapping** — Reinhard / ACES filmic, PBR·IBL과 세트
- **Bloom** — HDR 기반 Post-processing
- **PBR** — Physically Based Rendering (Cook-Torrance BRDF)
- **IBL** — Image Based Lighting (HDR 환경맵, Irradiance Map, Prefiltered Map)
- **Instancing** — `glDrawElementsInstanced` 기반 대량 오브젝트 렌더링
- **Frustum Culling** — View frustum vs AABB 교차 테스트
- **BVH** — Bounding Volume Hierarchy 기반 가속 구조 (Ray-AABB 교차 테스트, 공간 분할)

---

## 아키텍처 메모

- **Static Factory Pattern** — 모든 리소스 클래스(`Shader`, `Program`, `Buffer`, `Texture` 등)는 `Create()` / `CreateFromXxx()` 정적 팩토리 메서드를 통해서만 생성 가능. 생성자는 `private`.
- **RAII** — 모든 OpenGL 리소스(`glDeleteShader`, `glDeleteProgram`, `glDeleteBuffers` 등)는 소멸자에서 해제.
- **UniquePtr 기반 소유권** — `CLASS_PTR` 매크로로 `UPtr / SPtr / WPtr` 타입 alias 자동 생성.
- **의존성 분리** — 각 라이브러리는 독립된 `.cmake` 파일로 관리.
