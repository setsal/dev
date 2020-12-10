import sys
import yaml

path = "/home/setsal/project/sqlab/fuzzer/myfuzzer/yaml/live555.yaml"

if __name__ == '__main__':
    if len(sys.argv) <= 1:
        sys.exit(0)
    with open(path, "r") as stream:
        data = yaml.load(stream, Loader=yaml.CLoader)
    if sys.argv[1] == "fuzzer.protocol":
        print(data['fuzzer']['protocol'], end='')
    elif sys.argv[1] == "fuzzer.dictionary":
        print(data['fuzzer']['dictionary'], end='')
    elif sys.argv[1] == "fuzzer.ip_address":
        print(data['fuzzer']['ip_address'], end='')
    elif sys.argv[1] == "fuzzer.port":
        print(data['fuzzer']['port'], end='') 
    elif sys.argv[1] == "fuzzer.init_waitTime":
        print(data['fuzzer']['init_waitTime'], end='')
    elif sys.argv[1] == "fuzzer.state_selection_algo":
        print(data['fuzzer']['state_selection_algo'], end='')                           
    elif sys.argv[1] == "fuzzer.seed_selection_algo":
        print(data['fuzzer']['seed_selection_algo'], end='')
    elif sys.argv[1] == "info.protocol":
        print(data['info']['protocol'], end='')