import sys, math
EPSILON = 0.001
DEFAULT_ITER = 400

def calculate_euclidean_distance(p, q):
    """ Returns the Euclidean distance between two points p and q. """
    return math.sqrt(sum((p_i - q_i) ** 2 for p_i, q_i in zip(p, q)))
    
def assign_clusters(k, datapoints, centroids):
    """ Returns array assignments where assignments[i] stores the index of the cluster datapoints[i] is assigned to. """
    return [min(range(k), key=lambda i: calculate_euclidean_distance(datapoint, centroids[i])) for datapoint in datapoints]

def calculate_updated_centroids(k, d, datapoints, assignments, old_centroids):
    """
    Returns the updated centroids.
    If a centroid is empty, the previous centroid is reused.
    """
    updated_centroids = [[0.0] * d for _ in range(k)] 
    counts = [0] * k

    for point, cluster_index in zip(datapoints, assignments):
        for i in range(d):
            updated_centroids[cluster_index][i] += point[i]
        counts[cluster_index] += 1

    for i in range(k):
        if counts[i] == 0:
            # Reuse old centroid
            updated_centroids[i] = old_centroids[i][:] 
        else:
            updated_centroids[i] = [x / counts[i] for x in updated_centroids[i]]
    
    return updated_centroids

def calculate_centroids_using_kmeans(k, maximum_iteration, datapoints):
    """ Returns centroids calculated using the k-means algorithm. """
    n, d = len(datapoints), len(datapoints[0])
    previous_centroids = [[float('inf')] * d for _ in range(k)]
    centroids = [datapoints[i][:] for i in range(k)]
    assignments = [0] * n

    for i in range(maximum_iteration):
        if all(calculate_euclidean_distance(curr, prev) < EPSILON for curr, prev in zip(centroids, previous_centroids)):
            break
        
        previous_centroids = [centroid[:] for centroid in centroids]
        
        assignments = assign_clusters(k, datapoints, centroids)

        centroids = calculate_updated_centroids(k, d, datapoints, assignments, previous_centroids)

    return centroids


def error_exit():
        print("An Error Has Occurred")
        sys.exit(1)

def main():

    args = sys.argv[1:]

    # Validate argmument count
    if len(args) not in [1, 2]:
        error_exit()
    
    # Validate k is a natural number
    try:
        k_float = float(args[0])
        if not k_float.is_integer():
            raise ValueError
        k = int(k_float)
    except ValueError:
        print("Incorrect number of clusters!")
        sys.exit(1)

    # Validate iter is a natural number
    if len(args) == 2:
        try:
            iter_float = float(args[1])
            if not iter_float.is_integer():
                raise ValueError
            maximum_iteration = int(iter_float)
        except ValueError:
            print("Incorrect maximum iteration!")
            sys.exit(1)
    else:
        maximum_iteration = DEFAULT_ITER

    # Validate 1 < iter < 1000
    if not 1 < maximum_iteration < 1000:
        print("Incorrect maximum iteration!")
        sys.exit(1)

    # Process and Validate datapoints
    datapoints = []
    d = None
    for line in sys.stdin:
        line = line.strip()
        if line == "":
            break

        try:
            point = list(map(float, line.split(',')))
            if len(point) == 0:
                raise ValueError
        except ValueError:
            print("An Error Has Occurred")
            sys.exit(1)

        if d is None:
            d = len(point)
        elif len(point) != d:
            print("An Error Has Occurred")
            sys.exit(1)

        datapoints.append(point)

    if not 1 < k < len(datapoints):
        print("Incorrect number of clusters!")
        sys.exit(1)

    # Calculate centroids using k-means algorithm
    centroids = calculate_centroids_using_kmeans(k, maximum_iteration, datapoints)

    # Print centroids formatted to 4 decimal places
    for centroid in centroids:
        print(','.join(f'{coord:.4f}' for coord in centroid))
        
    sys.exit(0)


if __name__ == "__main__":
    main()