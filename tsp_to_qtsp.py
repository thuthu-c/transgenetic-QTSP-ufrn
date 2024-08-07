import numpy as np

def eval(graph: np.ndarray, tour: list):
    tour_val = 0
    n = graph.shape[0]

    tour_val += graph[tour[n - 2]][tour[n - 1]][tour[0]]
    tour_val += graph[tour[n - 1]][tour[0]][tour[1]]

    for j in range(n - 2):
        tour_val += graph[tour[j]][tour[j + 1]][tour[j + 2]]

    return tour_val

def tsp_to_qtsp(tsp_matrix):
    n = len(tsp_matrix)
    qtsp_matrix = np.zeros((n, n, n))
    
    for k in range(n):
        for i in range(n):
            for j in range(n):
                qtsp_matrix[k, i, j] = tsp_matrix[k, i] + tsp_matrix[i, j]

    return qtsp_matrix

def matrix_to_cpp(three_dim_matrix):
    n = three_dim_matrix.shape[0]

    def row_to_str(two_dim_matrix, n):
        result = f'    new int*[{n}]{{\n'

        for row in two_dim_matrix:
            result += f"        new int[{n}]{{ {', '.join([str(r) for r in row])} }}\n"

        result += '   },'

        return result

    m = '\n'.join([row_to_str(two_dim_matrix, n) for two_dim_matrix in three_dim_matrix])

    return (
        f'new int**[{n}]{{\n'
        f'{m}'
        '\n};'
    )

# Example usage:
tsp_matrix = np.array([
    [0, 1, 9, 7, 5],
    [1, 0, 2, 10, 6],
    [9, 2, 0, 3, 8],
    [7, 10, 3, 0, 4],
    [5, 6, 8, 4, 0],
])

qtsp_matrix = tsp_to_qtsp(tsp_matrix)
print(matrix_to_cpp(qtsp_matrix))
print(f'value of tour 0, 1, 2, 3, 4: {eval(qtsp_matrix, [0, 1, 2, 3, 4])}')
print(f'value of tour 0, 2, 1, 3, 4: {eval(qtsp_matrix, [0, 2, 1, 3, 4])}')
