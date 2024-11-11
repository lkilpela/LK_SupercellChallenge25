# Coding Challenge

## Overview
This repository contains the solution for the coding challenge using SFML. The goal of this challenge is to create a simple game using the SFML library.

## Requirements
- SFML 2.5.1 or later
- CMake 3.10 or later
- A C++17 compatible compiler

## 🕹️ How to Play
- **Control**:
    - `W` or `↑` to move up,
    - `A` or `←` to move left
    - `S` or `↓` to move down
    - `D` or `→` to move right
- **Attack**:
    - `left mouse` or `space` to kill enemies
- **Pause game**:
    - `P` to pause game
- **Collect items**:
    -  Move over items to collect them


## Setup
To set up the project, follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/lkilpela/LK_SupercellChallenge25
    cd LK_SupercellChallenge25
    ```

2. Create a build directory & configure the project with CMake:
    ```sh
    cmake -B ./build
    ```

3. Build the project:
    ```sh
    cmake --build ./build -- -j4
    ```

## Usage
To run the project, use the following command:
```sh
./survivor
```