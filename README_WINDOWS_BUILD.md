# Windows Build Guide

이 프로젝트는 Windows에서 Qt 6 + Npcap SDK로 빌드하도록 포팅되어 있습니다.

## 필요 항목

- Visual Studio 2022 C++ build tools
- Qt 6.8 이상, MSVC 2022 64-bit kit
- Npcap runtime
- Npcap SDK

Npcap runtime만 설치하면 실행 환경은 준비되지만, 컴파일에 필요한 `pcap.h`, `wpcap.lib`, `Packet.lib`는 SDK에 들어 있습니다.

## 빌드

Npcap SDK를 예를 들어 `C:\dev\npcap-sdk`에 압축 해제한 뒤 실행합니다.

```powershell
cmake -S . -B build-windows -A x64 -DNPCAP_ROOT=C:/dev/npcap-sdk
cmake --build build-windows --config Release
```

환경 변수로 지정해도 됩니다.

```powershell
$env:NPCAP_ROOT = "C:\dev\npcap-sdk"
cmake -S . -B build-windows -A x64
cmake --build build-windows --config Release
```

## 실행 및 배포

실시간 캡처는 관리자 권한 실행이 필요할 수 있습니다.

Qt DLL 배포가 필요하면 Qt MSVC kit의 `windeployqt`를 사용합니다.

```powershell
windeployqt build-windows\Release\appMini_Wireshark.exe
```

대상 PC에도 Npcap runtime이 설치되어 있어야 합니다.
