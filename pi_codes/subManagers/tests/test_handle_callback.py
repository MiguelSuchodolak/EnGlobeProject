import unittest, pytest

from subManagers.handle_callback import handleCallback

@pytest.mark.parametrize("read_value,expected_result", [(1,1.732223),(2,3.464446),(7,12.125561)])
def test_calculate_pressure(calculate_pressure, read_value, expected_result):
    assert calculate_pressure(read_value) == expected_result