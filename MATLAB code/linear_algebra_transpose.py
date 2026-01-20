import numpy as np

mat1 = np.array([[4,7,2],
             [3,5,8],
             [6,1,9]
              ])

def trans_matrix(matrix1):
    c = matrix1.T
    return(c)

print(mat1)
print(trans_matrix(mat1))
print(trans_matrix(trans_matrix(mat1)))

