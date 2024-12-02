# Automated Pedestrian Crossing Detection System

## Description

This project presents an automated system developed in C++ using OpenCV to analyze pedestrian crossing images from a vehicle’s perspective. Leveraging advanced computer vision techniques such as region and edge processing, the system accurately detects pedestrian crossings under varying conditions, including different lighting scenarios. The performance evaluation encompasses precision, recall, and robustness analysis across 20 static test images.

## Key Features

- **Efficient Detection Pipeline:** Designed and implemented a robust pipeline to effectively locate pedestrian crossings in diverse environments.
- **Environmental Adaptability:** Ensured the system adapts to various environmental factors, particularly lighting variability, enhancing detection accuracy.
- **Comprehensive Testing:** Conducted extensive testing with both provided and unseen datasets, offering detailed performance metrics.
- **Result Analysis:** Analyzed detection outcomes, highlighting successes and failures with in-depth reasoning and suggestions for improvements.

## Challenges & Learning Outcomes

- **Accuracy vs. Efficiency:** Explored the balance between achieving high detection accuracy and maintaining computational efficiency.
- **OpenCV Proficiency:** Gained in-depth experience in applying OpenCV for complex, real-world image processing tasks.
- **Parameter Tuning:** Enhanced understanding of parameter tuning and ensuring robustness in computer vision applications.

## Technical Skills

- **Programming Language:** C++
- **Libraries:** OpenCV
- **Tools:** Custom testing frameworks for automated performance evaluation

## Getting Started

### Prerequisites

- C++ compiler (e.g., g++, clang++)
- OpenCV library installed

### How to run this project

1. **Clone the Repository**
    ```bash
    git clone https://github.com/yourusername/pedestrian-crossing-detection.git
    cd pedestrian-crossing-detection
    ```

2. **Build the Project**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

### Usage

Run the executable with the path to the image you want to analyze:
```bash
./pedestrian_crossing_detector path_to_image.jpg
