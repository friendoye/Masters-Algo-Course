import sys
import functools
import logging
import traceback

import collections

def handle_error(re_raise=True, log_traceback=True, exc_type=Exception):
    _check_exc_type_param(exc_type)

    def wrapper(fun):
        @functools.wraps(fun)
        def wrapped(*args, **kwargs):
            try:
                return fun(*args, **kwargs)
            except:
                actual_exc_type, exc_instance, exc_traceback = sys.exc_info()
                _handle_raised_error(actual_exc_type, exc_instance, exc_traceback,\
                                     re_raise, log_traceback, exc_type)
        return wrapped

    return wrapper

class handle_error_context(object):
    def __init__(self, re_raise=True, log_traceback=True, exc_type=Exception):
        _check_exc_type_param(exc_type)

        self.re_raise = re_raise
        self.log_traceback = log_traceback
        self.exc_type = exc_type

    def __enter__(self):
        pass

    def __exit__(self, exc_type, exc_val, exc_tb):
        if exc_val is None:
            return True
        return _handle_raised_error(exc_type, exc_val, exc_tb,
                                    self.re_raise, self.log_traceback, self.exc_type)

def _check_exc_type_param(exc_type):
    assert (not isinstance(exc_type, tuple) and issubclass(exc_type, Exception)) or \
           (isinstance(exc_type, tuple) and all(issubclass(t, Exception) for t in exc_type)), \
        "exc_type should extend from Exception or be non empty tuple of Exception subclasses"

def _handle_raised_error(exc_type, exc_val, exc_tb,
                         re_raise, log_traceback, expected_exc_type):
    if (not isinstance(exc_type, tuple) and issubclass(exc_type, expected_exc_type)) or \
        (isinstance(expected_exc_type, collections.Iterable) and exc_type in expected_exc_type):
        if log_traceback:
            tb_list = traceback.format_tb(exc_tb)
            logging.error("".join(tb_list))
        if re_raise:
            raise
        return True
    else:
        raise