#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int debug = 0; // debug flag

/* ------------------------------------ */
/* Structs                              */
/* ------------------------------------ */

/* Struct for data */
typedef struct {
    double* x1;      
    double* x2;
    int* labels;
    int n_rows;      
} Data;

/* Perceptron Struct */
typedef struct {
    double* weights;
    double bias;
} Perceptron;



/* ------------------------------------ */
/* File and Memory Functions            */
/* ------------------------------------ */

/* Function to count rows of files */
int count_rows(const char* filename) {
    FILE* file = fopen(filename, "r");
    int rows = 0;
    char line[1024];
    
    while (fgets(line, 1024, file)) {
        rows++;
    }
    
    fclose(file);
    return rows;
}

/* Function to clear system memory from csv reads */
void free_data(Data* data) {
    free(data->x1);
    free(data->x2);
    free(data->labels);
}

/* Function to read data from csv files */
void read_data(const char* filename, Data* data) {
    FILE* file = fopen(filename, "r");
    char line[1024];
    int row = 0;
    
    while (fgets(line, 1024, file)) {
        if (row == 0) {
            row++;
            continue;
        }
        
        // split the line by commas into tokens

        char* token = strtok(line, ","); 
        data->x1[row - 1] = atof(token); // token 1 is the x1 value
        token = strtok(NULL, ",");
        data->x2[row - 1] = atof(token); // token 2 is the x2 value
        token = strtok(NULL, ",");
        data->labels[row - 1] = atoi(token); // token 3 is the label
        row++;
    }
    
    fclose(file);
}



/* ------------------------------------ */
/* Perceptron Functions                 */
/* ------------------------------------ */

/* Perceptron Initialization function */
void init_perceptron(Perceptron* perceptron, int n_features) {
    perceptron->weights = (double*)malloc(n_features * sizeof(double));
    for (int i = 0; i < n_features; i++) {
        perceptron->weights[i] = ((double)rand() / RAND_MAX) * 2 - 1;
    }
    perceptron->bias = ((double)rand() / RAND_MAX) * 2 - 1;
}

/* Perceptron predict function */
void predict(Perceptron* perceptron, Data* data, int* predictions) {
    double activation = 0.0;
    
    for (int i = 0; i < data->n_rows; i++) {
        // apply the activation equation
        activation = perceptron->weights[0]*data->x1[i] + perceptron->weights[1]*data->x2[i] + perceptron->bias;
        // Return 1 if activation>= 0 and -1 elsewhere (step function)
        predictions[i] = activation > 0 ? 1 : -1;
    }
}

/* Perceptron training function */
void train_perceptron(Perceptron* perceptron, Data* train_data, double learning_rate, int max_iterations) {
    int misclassified = 0; // to calculate misclassifications
    int prediction; // to calculate predict label
    int break_iter = max_iterations; // save loop iteration that perceptron gives 100 accuracy on training
    double activation = 0.0; // to calculate activation equation

    // Train for max_iterations
    for(int i=0; i<max_iterations; i++){ 
        misclassified = 0; // Reset misclassifications counter

        for(int j=0; j< train_data->n_rows; j++)
        {
            // Single predict
            activation = perceptron->weights[0]*train_data->x1[j] + perceptron->weights[1]*train_data->x2[j] + perceptron->bias;
            prediction = activation > 0 ? 1 : -1;
            
            // Adjust the weights and bias
            if ((prediction != train_data->labels[j]))
            {
                // Adjust weight 1
                perceptron->weights[0]+= learning_rate*train_data->labels[j]*train_data->x1[j];
                // Adjust weight 2
                perceptron->weights[1]+= learning_rate*train_data->labels[j]*train_data->x2[j];
                // Adjust bias
                perceptron->bias+= learning_rate*train_data->labels[j];

                // capture the misclassification
                misclassified++;
            }
        }

        // debug print
        if(debug)
            printf("Iteration %d, Misclassified: %d\n", i, misclassified);

        // check convergence
        if (!misclassified) {
            break_iter = ++i;
            break;
        }

    }

    // print training status
    double train_accuracy;
    if(!misclassified)
        train_accuracy=1.; // this avoids division by zero
    else
        train_accuracy = (double)(train_data->n_rows - misclassified) / train_data->n_rows;

    printf("---\n\nTraining phase completed\nAccuracy=%.4f \nTotal Training Iterations=%d\n\n", train_accuracy*100, break_iter);
}

/* Free perceptron's allocated memory */
void free_perceptron(Perceptron* perceptron) {
    free(perceptron->weights);
}

/* ------------------------------------ */
/* Evaluation Function */
/* ------------------------------------ */
double eval_predictions(Data* data, int * predictions)
{
    int correct = 0; // to calculate correct predictions
    double accuracy = 0.; // to calculate accuracy
    for (int i=0; i<data->n_rows; i++)
    {
        if((predictions[i] == data->labels[i])) // check if prediction is correct
            correct++;
    }
    accuracy = (double)correct/data->n_rows; // calculate accuracy
    return accuracy;
}

/* ------------------------------------ */
/* Main Function                        */
/* ------------------------------------ */
int main(int argc, char** argv)
{
    printf("\n=== Starting new perceptron training run ===\n");
    FILE* fp = fopen("training_data.csv", "r");
 
    if (!fp)
    {
        printf("Can't open training file\n");
        return 1;
    }
    fclose(fp);

    // Read train data
    int n_rows = count_rows("training_data.csv");
    n_rows--; // copensate for header

    int n_features = 2;
    if(argc < 3)
    {
        printf("Usage: %s <learning_rate> <max_iterations> <debug_prints>\n", argv[0]);
        return 1;
    }
    else if(argc >= 4)
        debug = atoi(argv[3]);
    

    double learning_rate = atof(argv[1]);
    int max_iterations = atoi(argv[2]);
   

    Data train_data;

    // Allocate memory for train data
    train_data.x1 = (double*)malloc(n_rows * sizeof(double));
    train_data.x2 = (double*)malloc(n_rows * sizeof(double));
    train_data.labels = (int*)malloc(n_rows * sizeof(int));
    train_data.n_rows = n_rows;

    // Read train data
    read_data("training_data.csv", &train_data);

    printf("--- ---\n\nTrain data successfully read.\nRows: %d\n", n_rows);

    // debug row printing
    if(debug)
    {   
        printf("Printing train data\n");
        for(int i=0; i<n_rows; i++)
        {
            printf("Row %d: x1=%.4lf, x2=%.4lf, label=%d\n", i, train_data.x1[i], train_data.x2[i], train_data.labels[i]);
        }
    }

    // Initialize perceptron
    Perceptron perceptron;
    init_perceptron(&perceptron, n_features);

    // Train the perceptron
    train_perceptron(&perceptron, &train_data, learning_rate, max_iterations);
    printf("Learning Rate: %.6lf\nMax Iterations: %d\n", learning_rate, max_iterations);
    printf("Weights: %.10lf, %.10lf\nBias: %.10lf\n---\n\n", perceptron.weights[0], perceptron.weights[1], perceptron.bias);
    
    // Test the perceptron
    fp =  fopen("test_data.csv", "r");

    // Check if training file is open correctly or exists
    if(!fp)
    {
        printf("Can't open training file\n");
    }
    else  // continue testing if success
    {
        n_rows = count_rows("test_data.csv");
        n_rows--;
        printf("Test data successfully read.\nRows: %d\n", n_rows);
        Data test_data;
        test_data.x1 = (double*)malloc(n_rows * sizeof(double));
        test_data.x2 = (double*)malloc(n_rows * sizeof(double));
        test_data.labels = (int*)malloc(n_rows * sizeof(int));
        test_data.n_rows = n_rows;
        

        int* predicted_labels = (int*)malloc(n_rows * sizeof(int));

        // Read test data from .csv file
        read_data("test_data.csv", &test_data);

        // debug row printing
        if(debug)
        {   
            printf("Printing test data\n");
            for(int i=0; i<n_rows; i++)
            {
                printf("Row %d: x1=%.4lf, x2=%.4lf, label=%d\n", i, test_data.x1[i], test_data.x2[i], test_data.labels[i]);
            }
        }

        // predict test values
        predict(&perceptron, &test_data, predicted_labels);

        // evaluate the predictions
        double accuracy = eval_predictions(&test_data, predicted_labels);
        printf("-----\n\nTest phase ended\nPrediction Accuracy = %.4lf\n\n--- ---\n", accuracy*100);

        // debug label printing
        if(debug)
        {
            printf("Printing predictions\n");
            for(int i=0; i<n_rows; i++)
            {
                printf("Row %d: label=%d, predicted=%d\n", i, test_data.labels[i], predicted_labels[i]);
            }
        }

        // free memory
        free(predicted_labels);
        free_data(&test_data);
    }


    // free memory
    fclose(fp);
    fp = NULL;
    free_data(&train_data);
    free_perceptron(&perceptron);
    return 0;
}