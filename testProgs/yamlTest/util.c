#include "alloc-inl.h"
#include "util.h"

#include <yaml.h>

// garbage code :(
int parse_yaml_config(Conf *conf, char *path) {
    FILE * fp;
    // char rpath[2048];
    char buffer[100];
    // char *exist;
    
    // exist = realpath(path, rpath);  

    // if(!exist) {
    //     FATAL("Config %s file not found", rpath);
    // }

    fp = popen("python3 /home/setsal/project/sqlab/fuzzer/myfuzzer/parse.py info.protocol", "r");
    fgets(buffer,sizeof(buffer),fp);
    conf->iConf.protocol = strdup(buffer);  
    pclose(fp);

    fp = popen("python3 /home/setsal/project/sqlab/fuzzer/myfuzzer/parse.py fuzzer.dictionary", "r");
    fgets(buffer,sizeof(buffer),fp);
    conf->fConf.dictionary = strdup(buffer);
    pclose(fp);

    fp = popen("python3 /home/setsal/project/sqlab/fuzzer/myfuzzer/parse.py fuzzer.protocol", "r");
    fgets(buffer,sizeof(buffer),fp);
    conf->fConf.protocol = strdup(buffer);
    pclose(fp);

    fp = popen("python3 /home/setsal/project/sqlab/fuzzer/myfuzzer/parse.py fuzzer.ip_address", "r");
    fgets(buffer,sizeof(buffer),fp);
    conf->fConf.ip_address = strdup(buffer);
    pclose(fp);    

    fp = popen("python3 /home/setsal/project/sqlab/fuzzer/myfuzzer/parse.py fuzzer.port", "r");
    fgets(buffer,sizeof(buffer),fp);
    conf->fConf.port = atol(buffer);
    pclose(fp);    
    
    fp = popen("python3 /home/setsal/project/sqlab/fuzzer/myfuzzer/parse.py fuzzer.init_waitTime", "r");
    fgets(buffer,sizeof(buffer),fp);
    conf->fConf.init_waitTime = atol(buffer);
    pclose(fp); 


    fp = popen("python3 /home/setsal/project/sqlab/fuzzer/myfuzzer/parse.py fuzzer.state_selection_algo", "r");
    fgets(buffer,sizeof(buffer),fp);
    conf->fConf.state_selection_algo = atol(buffer);
    pclose(fp);


    fp = popen("python3 /home/setsal/project/sqlab/fuzzer/myfuzzer/parse.py fuzzer.seed_selection_algo", "r");
    fgets(buffer,sizeof(buffer),fp);
    conf->fConf.seed_selection_algo = atol(buffer);
    pclose(fp);    


    conf->fConf.region_level_mutation = 1;
    conf->fConf.state_aware_mode = 1;
    conf->fConf.terminate_child = 1;
    return 0;
}