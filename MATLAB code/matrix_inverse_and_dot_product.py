import numpy as np

A = np.array([[1,2],
             [4,5]
              ])

B = np.array([[3],
             [6]
              ])

def inv_matrix(matrix1):
    
    c = np.linalg.det(matrix1)

    if c == 0:
        return('Matrix not compatible')
    else:
        d = np.linalg.inv(matrix1)
        return(d)

def rows(matrix):
    a = np.array(matrix.shape)
    return(a[0])

def columns(matrix):
    a = np.array(matrix.shape)
    return(a[1])

def dotprod_matrix(matrix1,matrix2):
    
    if columns(matrix1) != rows(matrix2):
        return('Matrix not compatible')
    else:
        d = np.dot(matrix1,matrix2)
        return(d)

    
inv_mat = inv_matrix(A)
id_mat = dotprod_matrix(A,inv_mat)
dotprod_mat = dotprod_matrix(inv_mat,B)

print(dotprod_mat)
