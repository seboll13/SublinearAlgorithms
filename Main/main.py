from loguru import logger
import numpy as np
from helpers import timer
from float_complex import CFloatComplex
from object_parser import Parser
from vector_structure import CVector

@timer
def dot_prod(u: CVector, v: CVector, is_optimised: bool = True) -> CFloatComplex:
    """Computes the dot product of two vectors, i.e. the sum of products of
        each pair of elements from the two vectors.
    
    Parameters
    ----------
    u : CVector
        First vector
    v : CVector
        Second vector
    is_optimised : bool
        If True, use the optimised version of the dot product function
    
    Returns
    -------
    float
        The dot product of the two vectors
    """
    return u.__dotprod__(v, is_optimised)

@timer
def std_dot_prod(u: list, v: list) -> float:
    """Compute the standard dot product between two lists of numbers.
    
    Parameters
    ----------
    u : list
        First list of numbers
    v : list
        Second list of numbers
    
    Returns
    -------
    float
        The dot product of the two lists
    """
    assert len(u) == len(v), "The vectors must have the same length"
    return sum((u[i] * v[i] for i in range(len(u))))

@timer
def numpy_dot_prod(u: np.ndarray, v: np.ndarray) -> np.float64:
    """Compute the dot product of two vectors using NumPy.

    Parameters
    ----------
    u : np.ndarray
        First vector
    v : np.ndarray
        Second vector

    Returns
    -------
    np.float64
        The dot product of the two vectors
    """
    return np.dot(u, v)

def main():
    """Main function"""
    logger.debug('Parsing vectors')
    u_parser = Parser('vec', 'u')
    v_parser = Parser('vec', 'v')
    u = u_parser.parse_vector()
    v = v_parser.parse_vector()
    logger.debug('Computing C-lib dot product of vectors')
    dot_prod(u, v, is_optimised=False)
    logger.debug('Computing Optimised C-lib dot product of vectors')
    dot_prod(u, v)

    logger.debug('Running standard dot product')
    std_u = u_parser.read_vector()
    std_v = v_parser.read_vector()
    std_dot_prod(std_u, std_v)

    logger.debug('Moving to NumPy')
    np_u = np.array(std_u)
    np_v = np.array(std_v)
    numpy_dot_prod(np_u, np_v)

if __name__ == '__main__':
    main()
