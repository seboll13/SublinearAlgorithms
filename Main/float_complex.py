from ctypes import Structure, c_float

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