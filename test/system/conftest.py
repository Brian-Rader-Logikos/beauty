import pytest
from subprocess import Popen
import server_info

@pytest.fixture(scope="module")
def test_server():
    with open("stdout.log", "wb") as stdout_file:
        with open("stderr.log", "wb") as stderr_file:
            with Popen([server_info.server_path]) as server_process:
                print(f"\nSystem Test Server process ID is {server_process.pid}\n")
                yield server_process
                server_process.terminate()
                server_process.wait(10)
