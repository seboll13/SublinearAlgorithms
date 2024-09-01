from functools import wraps
from time import perf_counter
from loguru import logger

# timer decorator adapted to work with recursive functions
# Note: perf_counter measures relative time directly dependent on the CPU frequency
#       this leads to a more accurate measurement of the time elapsed
def timer(func) -> callable:
    """Print the runtime of the decorated function."""

    @wraps(func)
    def wrapper(*args, **kwargs) -> callable:
        if not hasattr(wrapper, "timer_start"):
            wrapper.timer_start = perf_counter()
            is_root_call = True
        else:
            is_root_call = False
        res = func(*args, **kwargs)
        if is_root_call:
            end = perf_counter()
            logger.info(
                f"Elapsed time of {func.__name__}: {(end - wrapper.timer_start):.3f} [s]"
            )
            del wrapper.timer_start
        return res

    return wrapper
