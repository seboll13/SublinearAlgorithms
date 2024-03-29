from ctypes import *
from float_complex import c_float_complex

libmain = CDLL("../C/libmain.so")


class CVector(Structure):
    """Gets the vector struct from the C implementation."""
    _fields_ = [("capacity", c_int), ("items", POINTER(c_float_complex)), ("name", c_char_p)]

    def __init__(self, capacity, items, name) -> None:
        self.capacity = capacity
        self.items = items
        self.name = name

        # Create a pointer to the CVector instance
        self.c_vector_ptr = POINTER(CVector)(self)
    

    ################## Vector Operations ##################
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
    

    def __mul__(self, scalar) -> 'CVector':
        """Multiply a vector by a scalar."""
        libmain.vector_scalar_mult.argstype = [POINTER(CVector), c_float_complex, c_bool]
        libmain.vector_scalar_mult.restype = POINTER(CVector)

        return libmain.vector_scalar_mult(self.c_vector_ptr, scalar).contents
    

    def __dotprod__(self, other) -> c_float_complex:
        """Compute the dot product of two vectors."""
        libmain.dot_product.argstype = [POINTER(CVector), POINTER(CVector)]
        libmain.dot_product.restype = c_float_complex

        # Create a pointer to the other CVector instance
        other_ptr = POINTER(CVector)(other)

        return libmain.dot_product(self.c_vector_ptr, other_ptr)
    

    def __vecprod__(self, other) -> 'CVector':
        """Compute the vector product of two vectors."""
        libmain.vector_product.argstype = [POINTER(CVector), POINTER(CVector)]
        libmain.vector_product.restype = POINTER(CVector)

        # Create a pointer to the other CVector instance
        other_ptr = POINTER(CVector)(other)

        return libmain.vector_product(self.c_vector_ptr, other_ptr).contents
    

    def __scalarproj__(self, other) -> c_float_complex:
        """Compute the scalar projection of two vectors."""
        libmain.scalar_projection.argstype = [POINTER(CVector), POINTER(CVector)]
        libmain.scalar_projection.restype = c_float_complex

        # Create a pointer to the other CVector instance
        other_ptr = POINTER(CVector)(other)

        return libmain.scalar_projection(self.c_vector_ptr, other_ptr)
    

    def __vecproj__(self, other) -> 'CVector':
        """Compute the vector projection of two vectors."""
        libmain.vector_projection.argstype = [POINTER(CVector), POINTER(CVector)]
        libmain.vector_projection.restype = POINTER(CVector)

        # Create a pointer to the other CVector instance
        other_ptr = POINTER(CVector)(other)

        return libmain.vector_projection(self.c_vector_ptr, other_ptr).contents
    

    def __angle__(self, other) -> c_float:
        """Compute the angle between two vectors."""
        libmain.angle_between.argstype = [POINTER(CVector), POINTER(CVector)]
        libmain.angle_between.restype = c_float

        # Create a pointer to the other CVector instance
        other_ptr = POINTER(CVector)(other)

        return libmain.angle_between(self.c_vector_ptr, other_ptr)
    

    def __norm__(self, deg) -> c_float:
        """Compute the norm of a vector."""
        libmain.vector_Lp_norm.argstype = [POINTER(CVector), c_bool]
        libmain.vector_Lp_norm.restype = c_float

        return libmain.vector_Lp_norm(self.c_vector_ptr, deg)
    

    ################## Vector Properties ##################
    def __eq__(self, other) -> bool:
        """Check if two vectors are equal."""
        libmain.check_vector_equality.argstype = [POINTER(CVector), POINTER(CVector)]
        libmain.check_vector_equality.restype = c_bool

        # Create a pointer to the other CVector instance
        other_ptr = POINTER(CVector)(other)

        return libmain.check_vector_equality(self.c_vector_ptr, other_ptr)
    

    def __str__(self):
        libmain.print_vector.argstype = [POINTER(CVector)]
        libmain.print_vector.restype = None
        libmain.print_vector(self)
        return ''