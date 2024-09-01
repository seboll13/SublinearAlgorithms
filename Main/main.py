from float_complex import CFloatComplex
from helpers import timer
from object_parser import Parser
from vector_structure import CVector

@timer
def dot_prod(u: CVector, v: CVector) -> CFloatComplex:
    """Computes the dot product of two vectors, i.e. the sum of products of
        each pair of elements from the two vectors.
    
    Parameters
    ----------
    u : CVector
        First vector
    v : CVector
        Second vector
    
    Returns
    -------
    float
        The dot product of the two vectors
    """
    return u.__dotprod__(v)

def main():
    """Main function"""
    u = Parser('vec', 'u').parse_vector()
    v = Parser('vec', 'v').parse_vector()
    print(dot_prod(u, v))

if __name__ == '__main__':
    main()
