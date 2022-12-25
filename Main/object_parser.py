from vector_structure import CVector, c_float_complex

class Parser:
    def __init__(self, type, name) -> None:
        self.type = type
        self.name = name
        self.path = f'../data/{type}_{name}.txt'
    

    def read(self) -> list:
        with open(self.path, 'r') as f:
            elems = f.readlines()
            return [complex(float(_)) for _ in elems]
    
    
    def parse(self) -> CVector:
        elems = self.read()
        _complex = [(vi.real, vi.imag) for vi in elems]
        v = [c_float_complex(vi_real, vi_imag) for vi_real, vi_imag in _complex]
        return CVector(capacity=len(v), items=(c_float_complex * len(v))(*v), name=self.name.encode('utf-8'))