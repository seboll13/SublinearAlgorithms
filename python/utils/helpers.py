"""A library of helper functions."""
from functools import wraps
from time import process_time
from loguru import logger

def timer(func) -> callable:
    """
    Timer decorator adapted to work with recursive functions which will print
    the runtime of the decorated function.
    
    Parameters
    ----------
    func : callable
        The function to be decorated.
    
    Returns
    -------
    callable
        A wrapped version of `func` that, when called, logs the CPU time taken
        by the function and returns the function's original result.
    
    Notes
    -----
    The timer uses `process_time` from the `time` module to measure CPU time, 
    which avoids any discrepancies that might arise from using `perf_counter`. 
    The latter measures wall-clock time and includes time spent while the program 
    is asleep or waiting, making it less suitable for precise time measurements 
    of code execution.
    """

    @wraps(func)
    def wrapper(*args, **kwargs) -> callable:
        if not hasattr(wrapper, "timer_start"):
            wrapper.timer_start = process_time()
            is_root_call = True
        else:
            is_root_call = False
        res = func(*args, **kwargs)
        if is_root_call:
            end = process_time()
            logger.info(
                f"Elapsed CPU time of {func.__name__}: {(end - wrapper.timer_start):.3f} [s]"
            )
            del wrapper.timer_start
        return res

    return wrapper
