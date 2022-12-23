from time import time
from ctypes import *

libmain = CDLL("../C/libmain.so")

DIM = 3


class c_float_complex(Structure):
    """Handle the complex type to match the C implementation."""
    _fields_ = [("real", c_float),
                ("imag", c_float)]


class Vector(Structure):
    _fields_ = [("capacity", c_int), ("items", POINTER(c_float_complex)), ("name", c_char_p)]


def timer(func):
    def wrapper(*args, **kwargs):
        start = time()
        result = func(*args, **kwargs)
        end = time()
        print(f'Operation took {(end - start):.3f} seconds.')
        return result
    return wrapper


def options_prompt():
    op = input('Choose a vector operation from the following: (0, add), (1, sub), (2, dot product), (3, vector_product), (4, L1 norm), (5, L2 norm), (6, L infty norm) => ')
    while op != '0':#not in [str(_) for _ in range(7)]:
        op = input('Usage: enter a single digit from 0 to 6.')
    return op


def get_two_vectors():
    u = input('Enter the first vector using the following format: (x1, x2, ..., xn) => ')
    v = input('Enter the second vector using the following format: (y1, y2, ..., yn) => ')
    elems_of_u = u[1:-1].split(',')
    elems_of_v = v[1:-1].split(',')
    u = [complex(float(_)) for _ in elems_of_u]
    v = [complex(float(_)) for _ in elems_of_v]
    assert len(u) == len(v), 'Vectors must be of the same dimension.'
    return u, v


def parse_vector(v):
    _complex = [(vi.real, vi.imag) for vi in v]
    v = [c_float_complex(vi_real, vi_imag) for vi_real, vi_imag in _complex]
    return Vector(capacity=len(v), items=(c_float_complex * len(v))(*v), name=b'V')


def perform_operation(op, u, v):
    match op:
        case '0':
            libmain.vector_add.argstype = [POINTER(Vector), POINTER(Vector), c_bool]
            libmain.vector_add.restype = POINTER(Vector)
            return libmain.vector_add(u, v, True)
        case _:
            print('Operation not yet implemented.')
    return


def print_vector(v):
    libmain.print_vector.argstype = [POINTER(Vector)]
    libmain.print_vector.restype = None
    libmain.print_vector(v)


@timer
def operate_on(u, v, op):
    u = parse_vector(u)
    v = parse_vector(v)
    return perform_operation(op, u, v)


if __name__ == "__main__":
    op = options_prompt()
    u, v = get_two_vectors()
    w = operate_on(u, v, op)
    print_vector(w)