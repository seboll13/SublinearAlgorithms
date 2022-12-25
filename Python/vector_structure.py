from ctypes import *

libmain = CDLL("../C/libmain.so")


class c_float_complex(Structure):
    """Handle the complex type to match the C implementation."""
    _fields_ = [("real", c_float),
                ("imag", c_float)]
    
    def __init__(self, real, imag) -> None:
        self.real = real
        self.imag = imag
    

    def __str__(self) -> str:
        if self.imag < 0:
            return f'{self.real} - {-self.imag}i'
        if self.imag == 0:
            return f'{self.real}'
        return f'{self.real} + {self.imag}i'


class CVector(Structure):
    """Gets the vector struct from the C implementation."""
    _fields_ = [("capacity", c_int), ("items", POINTER(c_float_complex)), ("name", c_char_p)]

    def __init__(self, capacity, items, name) -> None:
        self.capacity = capacity
        self.items = items
        self.name = name

        # Create a pointer to the CVector instance
        self.c_vector_ptr = POINTER(CVector)(self)
    

    def __add__(self, other) -> 'CVector':
        """Add two vectors together."""
        libmain.vector_add.argstype = [POINTER(CVector), POINTER(CVector), c_bool]
        libmain.vector_add.restype = POINTER(CVector)

        # Create a pointer to the other CVector instance
        other_ptr = POINTER(CVector)(other)

        return libmain.vector_add(self.c_vector_ptr, other_ptr, True).contents
    

    def __sub__(self, other) -> 'CVector':
        """Subtract two vectors."""
        libmain.vector_add.argstype = [POINTER(CVector), POINTER(CVector), c_bool]
        libmain.vector_add.restype = POINTER(CVector)

        # Create a pointer to the other CVector instance
        other_ptr = POINTER(CVector)(other)

        return libmain.vector_add(self.c_vector_ptr, other_ptr, False).contents
    

    def __dotprod__(self, other) -> c_float_complex:
        """Compute the dot product of two vectors."""
        libmain.dot_product.argstype = [POINTER(CVector), POINTER(CVector)]
        libmain.dot_product.restype = c_float_complex

        # Create a pointer to the other CVector instance
        other_ptr = POINTER(CVector)(other)

        return libmain.dot_product(self.c_vector_ptr, other_ptr)
    

    def __str__(self):
        libmain.print_vector.argstype = [POINTER(CVector)]
        libmain.print_vector.restype = None
        libmain.print_vector(self)
        return ''