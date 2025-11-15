# Create library from all your Sudoku solver files
add_library(Code_library
        DancingLinks.cpp
        DancingLinks.h
        SudokuSolver.cpp
        SudokuSolver.h
        SudokuUI.cpp
        SudokuUI.h
)

# Make headers accessible to this library and all targets that link to it
target_include_directories(Code_library PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# Set C++ standard for the library
set_target_properties(Code_library PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)
