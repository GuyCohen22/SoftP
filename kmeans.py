import sys

# Process and Validate points
points = []
for line in sys.stdin:
    line = line.strip()
    if line == '':
        break
    try:
        point = list(map(float, line.split(',')))
        if len(point) == 0:
            raise ValueError
    except ValueError:
        print("An Error Has Occurred")
        sys.exit(1)
    points.append(point)

# Check that input is not empty
if len(points) == 0:
    print("An Error Has Occurred")
    sys.exit(1)

N = len(points)

def error_exit():
    print("An Error Has Occurred")
    sys.exit(1)

# Validate argmument count
if sys.argv != 3:
    error_exit()

# Validate K
if not sys.argv[1].isdigit():
    print("Incorrect number of clusters!")
    sys.exit(1)
    
try:
    K = int(sys.argv[1])
except ValueError:
    print("Incorrect number of clusters!")
    sys.exit(1)

# if not 1 < K < N:



# Validate iter
if not sys.argv[2].isdigit():
    print("Incorrect maximum iteration!")
try:
    iter = int(sys.argv[2])
except ValueError:
    print("Incorrect maximum iteration!")
    sys.exit(1)


