import server_info
    
def test_server_version():
    assert server_info.version == "0.1-rc"

def test_server_running(test_server):
    assert test_server.poll() == None
