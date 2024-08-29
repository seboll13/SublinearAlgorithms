import numpy as np
from time import time
from object_parser import Parser


def timer(func: callable) -> callable:
    """Timer decorator"""
    def wrapper(*args, **kwargs) -> callable:
        start = time()
        result = func(*args, **kwargs)
        end = time()
        print(f'Operation took {(end - start):.3f} seconds.')
        return result
    return wrapper


@timer
def numpy_mult(A, B):
    return A*B


@timer
def compute(A, B):
    return A*B


if __name__ == '__main__':
    # MATRIX TESTS
    print('Starting matrix tests...')
    A = Parser('mat', 'A').parse_matrix()
    B = Parser('mat', 'B').parse_matrix()
    print('Finished parsing matrices. Starting matrix multiplication...')
    C = compute(A, B)
    print(C)

    # NUMPY REFERENCE
    print('Starting numpy reference...')
    A = np.loadtxt('../data/mat_A.txt', dtype=np.complex128)
    B = np.loadtxt('../data/mat_B.txt', dtype=np.complex128)
    print('Finished parsing numpy data. Starting numpy multiplication...')
    numpy_mult(A, B)

    print('Done.')