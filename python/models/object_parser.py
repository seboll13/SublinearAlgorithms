from pathlib import Path
from python.models.vector import CVector
from python.models.matrix import CMatrix
from python.utils.float_complex import CFloatComplex

DATA_PATH = Path(__file__).parent.parent.parent / "data"


class Parser:
    """Parser class to read and parse objects from the data files."""

    def __init__(self, object_type, name) -> None:
        self.name = name
        self.object_type = object_type
        self.path = DATA_PATH / f"{object_type}_{name}.txt"
        self._vector_cache = None

    def read_vector(self) -> list:
        """Read a list of numbers from a file and returns it as a list of complex numbers."""
        if self._vector_cache is None:
            with open(self.path, "r", encoding="utf-8") as f:
                elems = f.readlines()
                self._vector_cache = [complex(float(_)) for _ in elems]
        return self._vector_cache

    def parse_vector(self) -> CVector:
        """Parse a vector from a file and returns it as a CVector instance."""
        elems = self.read_vector()  # will use cached data if available
        _complex = [(vi.real, vi.imag) for vi in elems]
        v = [CFloatComplex(vi_real, vi_imag) for vi_real, vi_imag in _complex]
        return CVector(
            capacity=len(v),
            items=(CFloatComplex * len(v))(*v),
            name=self.name.encode("utf-8"),
        )

    def read_matrix(self) -> list:
        """Read a matrix from a file and returns it as a list of lists of complex numbers."""
        with open(self.path, "r", encoding="utf-8") as f:
            elems = f.readlines()
            return [[complex(float(_)) for _ in row.split()] for row in elems]

    def parse_matrix(self) -> CMatrix:
        """Parse a matrix from a file and returns it as a CMatrix instance."""
        elems = self.read_matrix()
        _complex = [[(vij.real, vij.imag) for vij in row] for row in elems]
        v = [
            [CFloatComplex(vij_real, vij_imag) for vij_real, vij_imag in row]
            for row in _complex
        ]
        return CMatrix(
            rows=len(v),
            cols=len(v[0]),
            items=(CVector * len(v))(
                *[
                    CVector(
                        capacity=len(v[0]),
                        items=(CFloatComplex * len(v[0]))(*row),
                        name=self.name.encode("utf-8"),
                    )
                    for row in v
                ]
            ),
            name=self.name.encode("utf-8"),
        )
