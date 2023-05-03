import pytest    
from subManagers.handle_callback import handleCallback

@pytest.fixture
def dummy_handler():
    return handleCallback()
