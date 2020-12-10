#ifndef __UTIL_H
#define __UTIL_H 1

#include <yaml.h>

/* Parse yaml config */

typedef struct InfoConf {
    char *description;
    char *version;
    char *title;
    char *protocol;
}InfoConf;

typedef struct FuzzerConf {
    u8 *dictionary;
    u8 dirty_mode;
    u32  init_waitTime;
    u8 terminate_child;
    u8 state_selection_algo;
    u8 seed_selection_algo;
    u8 region_level_mutation;
    u8 state_aware_mode;
    char *protocol;
    u8 *ip_address;
    u32 port;
}FuzzerConf;

typedef struct ServerConf {
    char *protocol;
    char *ip_address;
    char port;
}ServerConf;

typedef struct ProtocolConf {
    char *command;
}ProtocolConf;

typedef struct Conf {
    char *version;
    struct InfoConf iConf;
    struct FuzzerConf fConf;
    struct ProtocolConf pConf;
}Conf;


/* Our example parser states. */
enum state_value {
    START,
    ACCEPT_VERSION,
    ACCEPT_INFO,
    ACCEPT_FUZZER,
    ACCEPT_SECTION,
    ACCEPT_LIST,
    ACCEPT_VALUES,
    ACCEPT_KEY,
    ACCEPT_VALUE,
    STOP,
    ERROR,
};

struct parser_state {
    enum state_value state;
    enum state_value cur_section;
    int accepted;
    int error;
    char *key;
    char *value;
    struct Conf data;
};

int consume_event(struct parser_state *s, yaml_event_t *event);

int parse_yaml_config(Conf *conf, char *path);

Conf* readConf(char *filename);


#endif /* __UTIL_H */
