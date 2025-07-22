"""A library of helper functions."""

from functools import wraps
from time import process_time
from typing import Callable
from loguru import logger


def timer(func) -> Callable:
    """
    Timer decorator adapted to work with recursive functions which will print
    the runtime of the decorated function.

    Parameters
    ----------
    func : Callable
        The function to be decorated.

    Returns
    -------
    Callable
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
    def wrapper(*args, **kwargs) -> Callable:
        if not hasattr(wrapper, "timer_start"):
            setattr(wrapper, "timer_start", process_time())
            is_root_call = True
        else:
            is_root_call = False
        res = func(*args, **kwargs)
        if is_root_call:
            end = process_time()
            logger.info(
                f"Elapsed CPU time of {func.__name__}: {(end - getattr(wrapper, 'timer_start'))*1000:.3f} [ms]"
            )
            delattr(wrapper, "timer_start")
        return res

    return wrapper
