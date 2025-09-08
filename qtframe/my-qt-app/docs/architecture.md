# Architecture

## Folder Architecture

    my-qt-app/
    │── CMakeLists.txt
    │── src/
    │   ├── main.cpp
    │   ├── ui/          # UI classes (windows, dialogs)
    │   │   └── mainwindow.cpp/.h
    │   ├── core/        # Business/domain logic
    │   ├── data/        # Persistence, network, APIs
    │   └── resources/   # Icons, qrc files
    │── include/         # Public headers
    │── tests/           # Unit/UI tests
    │── docs/
    │   └── architecture.md
    │── build/           # Out-of-source build dir
