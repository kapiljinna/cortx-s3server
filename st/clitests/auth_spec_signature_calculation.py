import os
import yaml
import pprint
import http.client, urllib.parse
import json
import xmltodict
from s3client_config import S3ClientConfig

class AuthHTTPClient:
    def __init__(self):
        self.iam_uri = S3ClientConfig.iam_uri

    def authenticate_user(self, headers, body):
        conn = http.client.HTTPConnection(urllib.parse.urlparse(self.iam_uri).netloc)
        conn.request("POST", "/", urllib.parse.urlencode(body), headers)
        response_data = conn.getresponse().read()
        conn.close()
        return response_data

def check_response(expected_response, test_response):
    assert test_response.decode("utf-8") in expected_response
    print("Response has [%s]." % (test_response))

test_data = {}
test_data_file = os.path.join(os.path.dirname(__file__), 'auth_spec_signcalc_test_data.yaml')
with open(test_data_file, 'r') as f:
    test_data = yaml.safe_load(f)

for test in test_data:
    print("Test case [%s] - " % test_data[test]['test-title'])
    headers = test_data[test]['req-headers']
    params = test_data[test]['req-params']
    expected_response = test_data[test]['output']
    test_response = AuthHTTPClient().authenticate_user(headers, params)
    check_response(expected_response, test_response)
    print("Test was successful\n")