from ctypes import CDLL, POINTER, Structure, c_bool, c_char_p, c_int
from python.models.vector import CVector
from python.utils.float_complex import CFloatComplex

libmain = CDLL('../src/libmain.so')


class CMatrix(Structure):
    """Structure to represent a matrix."""
    _fields_ = [('rows', c_int), ('cols', c_int), ('items', POINTER(CVector)), ('name', c_char_p)]


    def __init__(self, rows, cols, items, name) -> None:
        self.rows = rows
        self.cols = cols
        self.items = items
        self.name = name

        # Create a pointer to the CMatrix instance
        self.c_matrix_ptr = POINTER(CMatrix)(self)


    ################## Matrix Operations ##################
    def __add__(self, other) -> 'CMatrix':
        """Add two matrices together."""
        libmain.matrix_add.argstype = [POINTER(CMatrix), POINTER(CMatrix), c_bool]
        libmain.matrix_add.restype = POINTER(CMatrix)

        # Create a pointer to the other CMatrix instance
        other_ptr = POINTER(CMatrix)(other)

        return libmain.matrix_add(self.c_matrix_ptr, other_ptr, True).contents


    def __sub__(self, other) -> 'CMatrix':
        """Subtract two matrices."""
        libmain.matrix_add.argstype = [POINTER(CMatrix), POINTER(CMatrix), c_bool]
        libmain.matrix_add.restype = POINTER(CMatrix)

        # Create a pointer to the other CMatrix instance
        other_ptr = POINTER(CMatrix)(other)

        return libmain.matrix_add(self.c_matrix_ptr, other_ptr, False).contents


    def __mul__(self, other) -> 'CMatrix':
        """Multiply two matrices together."""
        libmain.matrix_mult.argstype = [POINTER(CMatrix), POINTER(CMatrix)]
        libmain.matrix_mult.restype = POINTER(CMatrix)

        # Create a pointer to the other CMatrix instance
        other_ptr = POINTER(CMatrix)(other)

        return libmain.matrix_mult(self.c_matrix_ptr, other_ptr).contents


    def __scalar_mult__(self, scalar) -> 'CMatrix':
        """Multiply a matrix by a scalar."""
        libmain.matrix_scalar_mult.argstype = [POINTER(CMatrix), CFloatComplex, c_bool]
        libmain.matrix_scalar_mult.restype = POINTER(CMatrix)

        return libmain.matrix_scalar_mult(self.c_matrix_ptr, scalar).contents


    def __str__(self) -> str:
        """Print the matrix in a nice format."""
        libmain.print_matrix.argstype = [POINTER(CMatrix)]
        libmain.print_matrix.restype = None
        libmain.print_matrix(self)
        return ''
