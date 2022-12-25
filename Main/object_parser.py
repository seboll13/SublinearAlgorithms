from vector_structure import CVector
from matrix_structure import CMatrix
from float_complex import c_float_complex

class Parser:
    def __init__(self, type, name) -> None:
        self.type = type
        self.name = name
        self.path = f'../data/{type}_{name}.txt'
    

    def read_vector(self) -> list:
        """Read a list of numbers from a file and returns it as a list of complex numbers."""
        with open(self.path, 'r') as f:
            elems = f.readlines()
            return [complex(float(_)) for _ in elems]
    
    
    def parse_vector(self) -> CVector:
        """Parse a vector from a file and returns it as a CVector instance."""
        elems = self.read_vector()
        _complex = [(vi.real, vi.imag) for vi in elems]
        v = [c_float_complex(vi_real, vi_imag) for vi_real, vi_imag in _complex]
        return CVector(capacity=len(v), items=(c_float_complex * len(v))(*v), name=self.name.encode('utf-8'))
    

    def read_matrix(self) -> list:
        """Read a matrix from a file and returns it as a list of lists of complex numbers."""
        with open(self.path, 'r') as f:
            elems = f.readlines()
            return [[complex(float(_)) for _ in row.split()] for row in elems]
    

    def parse_matrix(self) -> CMatrix:
        """Parse a matrix from a file and returns it as a CMatrix instance."""
        elems = self.read_matrix()
        _complex = [[(vij.real, vij.imag) for vij in row] for row in elems]
        v = [[c_float_complex(vij_real, vij_imag) for vij_real, vij_imag in row] for row in _complex]
        return CMatrix(rows=len(v), cols=len(v[0]), items=(CVector * len(v))(*[CVector(capacity=len(v[0]), items=(c_float_complex * len(v[0]))(*row), name=self.name.encode('utf-8')) for row in v]), name=self.name.encode('utf-8'))