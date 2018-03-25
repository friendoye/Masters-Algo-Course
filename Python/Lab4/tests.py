import unittest
import os
import os.path
from .iterators import unique, transpose
from .functional import scalar_product, flatten, walk_files
from .error_handling import handle_error, handle_error_context


class TestIterators(unittest.TestCase):
    def test_unique(self):
        expected = [1, 2, 3]
        actual = unique([1, 2, 1, 3, 2])
        self.assertEqual(expected, list(actual))

    def test_transpose(self):
        expected = [[1, 2], [-1, 3]]
        actual = transpose([[1, -1], [2, 3]])
        self.assertEqual(expected, list(map(list, actual)))


class TestFunctional(unittest.TestCase):
    def test_scalar_product_with_ints(self):
        expected = 1
        actual = scalar_product([1, 2], [-1, 1])
        self.assertEquals(expected, actual)

    def test_scalar_product_with_hex_string(self):
        expected = 5
        actual = scalar_product(['0x1', '0x2'], ['0x1', '0x2'])
        self.assertEquals(expected, actual)

    def test_scalar_product_with_binary_string(self):
        expected = 5
        actual = scalar_product(['0b1', '0b10'], ['0b1', '0b10'])
        self.assertEquals(expected, actual)

    def test_scalar_product_with_octal_string(self):
        expected = 5
        actual = scalar_product(['0o1', '0o2'], ['0o1', '0o2'])
        self.assertEquals(expected, actual)

    def test_scalar_product_with_float(self):
        expected = 4.25
        actual = scalar_product([1.5, 2.5], [2, 0.5])
        self.assertAlmostEqual(expected, actual)

    def test_scalar_product_with_strings(self):
        expected = 1
        actual = scalar_product(['1', '2'], ['-1', '1'])
        self.assertEqual(expected, actual)

    def test_scalar_product_with_mixed(self):
        expected = 1
        actual = scalar_product([1, '2'], ['-1', 1])
        self.assertEquals(expected, actual)

    def test_scalar_product_with_non_number_string(self):
        actual = scalar_product([1, 'abc'], [-1, 1])
        self.assertIsNone(actual)

    def test_flatten(self):
        expected = [1, 2, 0, 1, 1, 2, 1, 'ab']
        actual = flatten([1, 2, xrange(2), [[], [1], [[2]]], (x for x in [1]), 'ab'])
        self.assertEquals(expected, list(actual))

    @unittest.skip("taking sooo loooong")
    def test_walk_files_flatten(self):
        test_dir = os.path.expanduser("~")
        expected = sorted(os.listdir(test_dir))
        actual = sorted(flatten(walk_files(test_dir)))
        self.assertListEqual(expected, actual)


class TestErrorHandling(unittest.TestCase):
    def test_handle_error_context(self):
        def caller():
            with handle_error_context(log_traceback=True, exc_type=ValueError):
                raise ValueError()
        self.assertRaises(ValueError, caller)

    def test_decorator_handle_error(self):
        @handle_error(re_raise=False)
        def some_function():
            x = 1 / 0
        some_function()

    def test_decorator_handle_error_2(self):
        @handle_error(re_raise=False, exc_type=KeyError)
        def some_function():
            x = 1 / 0
        self.assertRaises(ZeroDivisionError, some_function)

    def test_decorator_handle_error_3(self):
        @handle_error(re_raise=False, exc_type=(KeyError, ZeroDivisionError))
        def some_function():
            raise KeyError
        some_function()

    def test_decorator_handle_error_4(self):
        @handle_error(re_raise=False, exc_type=(KeyError, ZeroDivisionError))
        def some_function():
            x = 1 / 0
        some_function()

    def test_decorator_handle_error_5(self):
        @handle_error(re_raise=False, exc_type=(KeyError, ValueError))
        def some_function():
            x = 1 / 0
        self.assertRaises(ZeroDivisionError, some_function)


if __name__ == '__main__':
    unittest.main()