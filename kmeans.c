#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const double EPSILON = 0.001;
const int DEFAULT_ITER = 400; 

typedef struct coordinate {
    double value;
    struct coordinate *next;
} Coordinate;

typedef struct vector
{
    struct vector *next;
    Coordinate *coordinates;
} Vector;

/* Returns 1 if a string is a natural number and 0 otherwise. accepts 81.0 as a natural number according to forum clarification */
int is_natural_number(const char *str) {
    int i = 0;
    int dot_seen = 0;

    if (str[0] == '\0') return 0;
    if (str[0] == '.' && str[1] == '\0') return 0;
    if (str[0] != '.' && (str[0] < '0' || str[0] > '9')) return 0;

    while (str[i] != '\0') {
        if (str[i] == '.') {
            if (dot_seen) return 0;
            dot_seen = 1;
        }

        else if (str[i] < '0' || str[i] > '9') {
            return 0;
        }

        else if (dot_seen && str[i] != '0') {
            return 0;
        }
        i++;
    }

    return i >= 1;
}

/* Frees the entire memory allocated to the matrix */
void free_matrix(Vector *head_vector) {
    Vector *vector_to_free;
    Coordinate *curr_coordinate, *coordinate_to_free;

    while (head_vector != NULL) {
        curr_coordinate = head_vector->coordinates;

        while (curr_coordinate != NULL) {
            coordinate_to_free = curr_coordinate;
            curr_coordinate = curr_coordinate->next;
            free(coordinate_to_free);
        }

        vector_to_free = head_vector;
        head_vector = head_vector->next;
        free(vector_to_free);
    }
}

/* Frees all memory allocated for a coordinate list starting from the given head */
void free_curr_coordinates(Coordinate *head_coordinate) {
    
}

/* */
void process_and_validate_datapoints(Vector **head_vector, int *num_vectors, int *dimension) {
    Vector *local_head_vector, *curr_vector, *next_vector;
    Coordinate *head_coordinate, *curr_coordinate, *next_coordinate;
    int i, j, rows, cols;
    double n;
    char c;

    head_coordinate = malloc(sizeof(Coordinate));
    if (head_coordinate == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);
    }
    curr_coordinate = head_coordinate;
    curr_coordinate->next = NULL;

    local_head_vector = malloc(sizeof(Vector));
    if (local_head_vector == NULL) {
        free(head_coordinate);
        printf("An Error Has Occurred\n");
        exit(1);
    }
    curr_vector = local_head_vector;
    curr_vector->next = NULL;

    while (scanf("%lf%c", &n, &c) == 2) {

        if (c == '\n') {
            curr_coordinate->value = n;
            curr_vector->coordinates = head_coordinate;
            curr_vector->next = malloc(sizeof(Vector));
            if (curr_vector->next == NULL) {
                free_matrix(local_head_vector);
            }
        }

    }



    
}




/*  Returns the Euclidean distance between two vectors */
double calculate_euclidean_distance(Vector *v1, Vector *v2) {
    double sum = 0.0, diff;
    Coordinate *c1 = v1->coordinates;
    Coordinate *c2 = v2->coordinates;
    
    while (c1 != NULL && c2 != NULL) {
        diff = c1->value - c2->value;
        sum += diff * diff;
        c1 = c1->next;
        c2 = c2->next;
    }
    
    return sqrt(sum);
}

int main(int argc, char **argv) {
    int k, maximum_iteration;
    int num_vectors, dimension;
    Vector *head_vector;

    /* Validate argument count */
    if (argc != 2 && argc != 3) {
        printf("An Error Has Occurred\n");
        exit(1);
    }

    /* Validate k is a natural number */
    if (!is_natural_number(argv[1])) {
        printf("Incorrect number of clusters!\n");
        exit(1);
    }

    k = atoi(argv[1]);

    /* Validate iter is a natural number */
    if (argc == 3 && !is_natural_number(argv[2])) {
        printf("Incorrect maximum iteration!\n");
        exit(1);
    }

    maximum_iteration = argc == 3 ? atoi(argv[2]) : DEFAULT_ITER;

    /* Validate 1 < iter < 1000 */
    if (!(1 < maximum_iteration && maximum_iteration < 1000)) {
        printf("Incorrect maximum iteration!\n");
        exit(1);
    }

    process_and_validate_datapoints(&head_vector, &num_vectors, &dimension);



    return 0;
}