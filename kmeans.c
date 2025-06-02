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

/* Returns 1 if a string is a natural number and 0 otherwise. accepts X.0 as a natural number according to forum clarification */
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
    Coordinate *coordinate_to_free;

    while (head_coordinate != NULL) {
        coordinate_to_free = head_coordinate;
        head_coordinate = head_coordinate->next;
        free(coordinate_to_free);
    }
}

/* Reads datapoints from stdin, builds a linked list matrix, and sets the number of vectors and dimension */
void process_datapoints(Vector **head_vector, int *num_vectors, int *dimension) {
    Vector *local_head_vector, *curr_vector;
    Coordinate *head_coordinate, *curr_coordinate;
    int rows = 0, cols = 0, cols_count = 1;
    double n;
    char c;

    head_coordinate = calloc(1, sizeof(Coordinate));
    if (head_coordinate == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);
    }
    curr_coordinate = head_coordinate;
    curr_coordinate->next = NULL;

    local_head_vector = calloc(1, sizeof(Vector));
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
            if (rows == 0) {
                cols = cols_count;
            }
            curr_vector->coordinates = head_coordinate;
            curr_vector->next = calloc(1, sizeof(Vector));
            if (curr_vector->next == NULL) {
                free_matrix(local_head_vector);
                printf("An Error Has Occurred\n");
                exit(1);
            }
            curr_vector = curr_vector->next;
            curr_vector->next = NULL;
            head_coordinate = calloc(1, sizeof(Coordinate));
            if (head_coordinate == NULL) {
                free_matrix(local_head_vector);
                printf("An Error Has Occurred\n");
                exit(1);
            }
            curr_coordinate = head_coordinate;
            curr_coordinate->next = NULL;
            rows++;
            cols_count = 1;
            continue;
        }

        curr_coordinate->value = n;
        curr_coordinate->next = calloc(1, sizeof(Coordinate));
        if (curr_coordinate->next == NULL) {
                free_matrix(local_head_vector);
                free_curr_coordinates(head_coordinate);
                printf("An Error Has Occurred\n");
                exit(1);
            }
        curr_coordinate = curr_coordinate->next;
        curr_coordinate->next = NULL;
        cols_count++;
    }

    *head_vector = local_head_vector;
    *num_vectors = rows;
    *dimension = cols;
    free(head_coordinate);
}
/* Allocates and returns a linked list of k centroids, each with the given dimension. returns NULL on failure. */
Vector *init_centroids_matrix(int k, int dimension) {
    Vector *head_vector, *curr_vector;
    Coordinate *head_coordinate, *curr_coordinate;
    int i, j;

    head_vector = calloc(1, sizeof(Vector));
    if (head_vector == NULL) {
        return NULL;
    }
    curr_vector = head_vector;

    for (i = 0; i < k; i++) {
        head_coordinate = calloc(1, sizeof(Coordinate));
        if (head_coordinate == NULL) {
            free_matrix(head_vector);
            return NULL;
        }
        curr_coordinate = head_coordinate;

        for (j = 1; j < dimension; j++) {
            curr_coordinate->next = calloc(1, sizeof(Coordinate));
            if (curr_coordinate->next == NULL) {
                free_matrix(head_vector);
                free_curr_coordinates(head_coordinate);
                return NULL;
            }
            curr_coordinate = curr_coordinate->next;
        }

        curr_vector->coordinates = head_coordinate;

        if (i != k - 1) {
            curr_vector->next = calloc(1, sizeof(Vector));
            if (curr_vector->next == NULL) {
                free_matrix(head_vector);
                return NULL;
            }
            curr_vector = curr_vector->next;
        }
    }
    return head_vector;
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

/* Initializes the centroids matrix with the first k datapoints */
void init_centroids_as_first_k_datapoints(Vector *datapoints, Vector *centroids, int k, int dimension){
    Vector *datapoints_curr_vector = datapoints, *centroids_curr_vector = centroids;
    Coordinate *datapoints_curr_coordinate, *centroids_curr_coordinate;
    int i, j;

    for (i = 0; i < k; i++) {
        datapoints_curr_coordinate = datapoints_curr_vector->coordinates;
        centroids_curr_coordinate = centroids_curr_vector->coordinates;
        for (j = 0; j < dimension; j++) {
            centroids_curr_coordinate->value = datapoints_curr_coordinate->value;
            datapoints_curr_coordinate = datapoints_curr_coordinate->next;
            centroids_curr_coordinate = centroids_curr_coordinate->next;
        }
        datapoints_curr_vector = datapoints_curr_vector->next;
        centroids_curr_vector = centroids_curr_vector->next;
    }
}

/* Sets all values in the centroids matrix to 0 */
void zero_out_centroids(Vector *centroids, int k, int dimension) {
    Vector *curr_vector = centroids;
    Coordinate *curr_coordinate;
    int i, j;

    for (i = 0; i < k; i++) {
        curr_coordinate = curr_vector->coordinates;
        for (j = 0; j < dimension; j++) {
            curr_coordinate->value = 0.0;
            curr_coordinate = curr_coordinate->next;
        }
        curr_vector = curr_vector->next;
    }
}

/* Returns 1 if the centroids have converged, 0 otherwise */
int has_converged(Vector *prev_centroids, Vector *curr_centroids, int k) {
    Vector *local_prev_centroids = prev_centroids, *local_curr_centroids = curr_centroids;
    double curr_distance;
    int i;

     for (i = 0; i < k; i++) {
        curr_distance = calculate_euclidean_distance(local_prev_centroids, local_curr_centroids);
        if (curr_distance >= EPSILON) return 0;
        local_prev_centroids = local_prev_centroids->next;
        local_curr_centroids = local_curr_centroids->next;
    }

    return 1;
}

/* Assigns a datapoint to the closest cluster and returns the index of that cluster */
int assign_datapoint_to_closest_cluster(Vector *point_to_assign, Vector *centroids, int k) {
    Vector *curr_centroid = centroids;
    int min_index = 0;
    double curr_distance;
    double min_distance = calculate_euclidean_distance(point_to_assign, curr_centroid);
    int i;

    for (i = 1; i < k; i++) {
        curr_centroid = curr_centroid->next;
        curr_distance = calculate_euclidean_distance(point_to_assign, curr_centroid);
        if (curr_distance < min_distance) {
            min_distance = curr_distance;
            min_index = i;
        }
    }

    return min_index;
}

/* Updates the current centroids based on the assignments and counts of datapoints in each cluster */
void update_curr_centroids(Vector *prev_centroids, Vector *curr_centroids, Vector *datapoints, int *assignments, int *counts, int k, int num_vectors, int dimension) {
    Vector *curr_datapoint = datapoints;
    Vector *curr_centroid, *prev_centroid;
    Coordinate *curr_datapoint_coordinate, *curr_centroid_coordinate, *prev_centroid_coordinate;
    int i, j, m;

    for (i = 0; i < num_vectors; i++) {
        curr_centroid = curr_centroids;
        for (j = 0; j < assignments[i]; j++) {
            curr_centroid = curr_centroid->next;
        }

        curr_datapoint_coordinate = curr_datapoint->coordinates;
        curr_centroid_coordinate = curr_centroid->coordinates;

        for (m = 0; m < dimension; m++) {
            curr_centroid_coordinate->value += curr_datapoint_coordinate->value;
            curr_centroid_coordinate = curr_centroid_coordinate->next;
            curr_datapoint_coordinate = curr_datapoint_coordinate->next;
        }
        curr_datapoint = curr_datapoint->next;
    }

    prev_centroid = prev_centroids;
    curr_centroid = curr_centroids;
    for (i = 0; i < k; i++) {
        prev_centroid_coordinate = prev_centroid->coordinates;
        curr_centroid_coordinate = curr_centroid->coordinates;
        
        for (j = 0; j < dimension; j++) {
            curr_centroid_coordinate->value = counts[i] > 0 ? (curr_centroid_coordinate->value) / counts[i] : prev_centroid_coordinate->value;
            prev_centroid_coordinate = prev_centroid_coordinate->next;
            curr_centroid_coordinate = curr_centroid_coordinate->next;
        }

        prev_centroid = prev_centroid->next;
        curr_centroid = curr_centroid->next;
    }
}

/* Returns centroids calculated using the k-means algorithm */
Vector *calculate_centroids_using_kmeans(int k, int maximum_iteration, int num_vectors, int dimension, Vector *datapoints, Vector *prev_centroids, Vector *curr_centroids, int *assignments, int*counts) {

    Vector *temp_centroids, *local_prev_centroids = prev_centroids, *local_curr_centroids = curr_centroids, *point_to_assign;
    int converged_flag = 0, iteration_cnt = 0;
    int i;

    init_centroids_as_first_k_datapoints(datapoints, local_prev_centroids, k, dimension);

    while (iteration_cnt < maximum_iteration && !converged_flag) {
        memset(assignments, 0, sizeof(int) * num_vectors);
        memset(counts, 0, sizeof(int) * k);
        zero_out_centroids(local_curr_centroids, k, dimension);
        point_to_assign = datapoints;

        /* Assign each datapoint to the closest cluster */
        for (i = 0; i < num_vectors; i++) {
            assignments[i] = assign_datapoint_to_closest_cluster(point_to_assign, local_prev_centroids, k);
            counts[assignments[i]]++;
            point_to_assign = point_to_assign->next;
        }

        /* Calculate new centroids */
        update_curr_centroids(local_prev_centroids, local_curr_centroids, datapoints, assignments, counts, k, num_vectors, dimension);
        converged_flag = has_converged(local_prev_centroids, local_curr_centroids, k);
        iteration_cnt++;
        if (!converged_flag) {
            /* Swap the previous and current centroids for the next iteration */
            temp_centroids = local_prev_centroids;
            local_prev_centroids = local_curr_centroids;
            local_curr_centroids = temp_centroids;
        }
    }
    
    return converged_flag ? local_curr_centroids : local_prev_centroids;
}

/* Prints the centroids matrix in the required format */
void print_result(Vector *result_centroids) {
    Vector *curr_vector = result_centroids;
    Coordinate *curr_coordinate;
    while (curr_vector != NULL) {
        curr_coordinate = curr_vector->coordinates;
        while (curr_coordinate != NULL) {
            printf("%.4f", curr_coordinate->value);
            if (curr_coordinate->next != NULL) printf(",");
            curr_coordinate = curr_coordinate->next;
        }
        printf("\n");
        curr_vector = curr_vector->next;
    }
}

int main(int argc, char **argv) {
    int k, maximum_iteration;
    int num_vectors, dimension;
    Vector *datapoints, *prev_centroids, *curr_centroids, *result_centroids;
    int *assignments, *counts;

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

    process_datapoints(&datapoints, &num_vectors, &dimension);

    if (!(1 < k && k < num_vectors)) {
        printf("Incorrect number of clusters!\n");
        free_matrix(datapoints);
        exit(1);
    }

    prev_centroids = init_centroids_matrix(k, dimension);
    curr_centroids = init_centroids_matrix(k, dimension);
    assignments = calloc(num_vectors, sizeof(int));
    counts = calloc(k, sizeof(int));
    if (prev_centroids == NULL || curr_centroids == NULL || assignments == NULL || counts == NULL) {
        free_matrix(datapoints);
        free_matrix(prev_centroids);
        free_matrix(curr_centroids);
        free(assignments);
        free(counts);
        printf("An Error Has Occurred\n");
        exit(1);
    }

    result_centroids = calculate_centroids_using_kmeans(k, maximum_iteration, num_vectors, dimension, datapoints, prev_centroids, curr_centroids, assignments, counts);
    print_result(result_centroids);

    free_matrix(datapoints);
    free_matrix(prev_centroids);
    free_matrix(curr_centroids);
    free(assignments);
    free(counts);

    return 0;
}