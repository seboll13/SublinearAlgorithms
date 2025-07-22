from loguru import logger
import numpy as np
from python.utils.helpers import timer
from python.utils.float_complex import CFloatComplex
from python.models.object_parser import Parser
from python.models.vector import CVector

DIM = int(1e7)


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
    return sum((u[i] * v[i] for i in range(len(u))))


@timer
def numpy_dot_prod(u: np.ndarray, v: np.ndarray) -> np.ndarray:
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
    return u @ v  # replaces np.dot(u, v)


def parse_or_read_vector(vec_name: str, read: bool = False) -> CVector | list:
    """Parse or read a vector from the command line.

    Parameters
    ----------
    vec_name : str
        The name of the vector
    read : bool
        Whether to read the vector from the command line. Default is parse.

    Returns
    -------
    CVector | list
        The list of complex numbers, as a CVector if read is False
    """
    parser = Parser("vec", vec_name)
    return parser.read_vector() if read else parser.parse_vector()


def main():
    """Main function"""
    logger.debug("Parsing vectors")
    u = parse_or_read_vector("u")
    v = parse_or_read_vector("v")
    logger.debug("Computing optimised C-lib dot product of vectors")
    dot_prod(u, v)

    logger.debug("Running standard dot product")
    std_u = parse_or_read_vector("u", True)
    std_v = parse_or_read_vector("v", True)
    std_dot_prod(std_u, std_v)

    logger.debug("Moving to NumPy")
    np_u = np.array(std_u)
    np_v = np.array(std_v)
    numpy_dot_prod(np_u, np_v)


if __name__ == "__main__":
    main()
