# A Simple Perceptron in C

This repository contains a simple perceptron implementation in C for binary classification. Created for a uni class and scored 100/100.

## Overview

The perceptron implementation includes:
- Data loading from CSV files
- Training with configurable learning rate and iteration count
- Testing and evaluation

## Requirements

- C compiler (gcc recommended)
- CSV data files with the following format:
  - First column: x1 (first feature)
  - Second column: x2 (second feature)
  - Third column: label (1 or -1)
  - First row should contain headers

## Usage

### Compilation

Compile the program using:

```bash
gcc -Wall -o perceptron perceptron.c
```

### Running the Program

Run the compiled program with the following arguments:

```bash
./perceptron <learning_rate> <max_iterations> [debug_prints]
```

Parameters:
- `learning_rate`: Step size for weight updates (e.g., 0.01)
- `max_iterations`: Maximum number of training iterations
- `debug_prints`: Optional (0 or 1) to enable verbose output

Example:
```bash
./perceptron 0.01 1000 0
```

### Input Data

The program expects two CSV files in the same directory:
- `training_data.csv`: For training the perceptron
- `test_data.csv`: For evaluating the trained model

## Implementation Details

### Data Structures

- `Data`: Stores input features and labels
- `Perceptron`: Contains weights and bias for the model

### Key Functions

- `init_perceptron()`: Initializes the perceptron with random weights
- `train_perceptron()`: Implements the perceptron learning algorithm
- `predict()`: Makes predictions using the trained model
- `eval_predictions()`: Calculates accuracy on test data

## Output

The program outputs:
- Training information (accuracy and iterations)
- Final model parameters (weights and bias)
- Test accuracy

## Memory Management

The program handles memory allocation and cleanup automatically to prevent leaks.

## Example Output

```
=== Starting new perceptron training run ===
--- ---

Train data successfully read.
Rows: 100
---

Training phase completed
Accuracy=100.0000 
Total Training Iterations=45

Learning Rate: 0.010000
Max Iterations: 1000
Weights: 0.4728371240, 0.7351962531
Bias: -0.2847293847
---

Test data successfully read.
Rows: 50
-----

Test phase ended
Prediction Accuracy = 97.5000

--- ---
```
