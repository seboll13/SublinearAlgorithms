from time import time
from object_parser import Parser


def timer(func: callable) -> callable:
    """Timer decorator"""
    def wrapper(*args, **kwargs) -> callable:
        start = time()
        result = func(*args, **kwargs)
        end = time()
        print(f'Operation took {(end - start):.3f} seconds.')
        return result
    return wrapper


if __name__ == '__main__':
    u = Parser('vec', 'U').parse()
    v = Parser('vec', 'V').parse()
    print(u.__dotprod__(v))