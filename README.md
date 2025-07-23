# Optimised C Library for Sublinear Algorithms

*This repository aims to provide an optimised C library for vector, matrix, and tensor structures. These low-level components will serve as the foundation for implementing and testing sublinear algorithms. The C code is accessed from Python via a C–Python interface.*

> ⚠️ Functional sublinear-time algorithms are **not yet implemented**. Before jumping into algorithms, the focus is on building a solid, efficient, and flexible low-level structure. Each commit documents progress, including refactoring, optimisation, and integration work. Algorithms will follow once the foundation is stable.

---

## Objectives

* Define an efficient vector structure.
* Build a matrix structure (built upon vector).
* Build a tensor structure (built upon matrix).
* Create helper functions for each structure.
* Implement test algorithms (initially in Python).
* Port these algorithms to C.
* Use a Python–C interface to benchmark and test.
* Optimise performance across the stack.

---

## SonarLint Integration (VS Code + macOS)

To enable proper linting and static analysis for C/C++ code via SonarLint:

1. Install Bear:

   ```bash
   brew install bear
   ```

2. Generate the compilation database:

   ```bash
   bear -- make -C src
   ```

   Or, to include tests and benchmarks:

   ```bash
   bear -- sh -c "make -C src && make -C tests && make -C benchmarks"
   ```

3. This will generate a `compile_commands.json` file in the root directory.

4. In VSCode, open your workspace settings (`.vscode/settings.json`) and add:

   ```json
   "sonarlint.pathToCompileCommands": "${workspaceFolder}/compile_commands.json"
   ```

5. Reload VS Code.

---

## Notes

This is a work in progress. Most of the effort so far is dedicated to performance tuning and structure definition.
