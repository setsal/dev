#include <yaml.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#define BUFFER_SIZE 65536
#define MAX_DOCUMENTS  16

typedef struct Conf {
    char *version;
    struct InfoConf *iConf;
    struct FuzzerConf *fConf;
    struct ProtocolConf *pConf;
}Conf;

typedef struct InfoConf {
    char *description;
    char *version;
    char *title;
    char *protocol;
}InfoConf;

typedef struct FuzzerConf {
    char *description;
    char *title;
}FuzzerConf;

typedef struct ProtocolConf {
    char *description;
    char *title;
}ProtocolConf;

/* Our example parser states. */
enum state_value {
    START,
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
    int accepted;
    int error;
    char *key;
    char *value;
    struct InfoConf data;
};


int main(int argc, char *argv[]) {

    // read yaml file
    FILE *file = fopen(argv[1], "r");
    assert(file);

    yaml_parser_t parser;
    yaml_token_t token;

    Conf *conf = malloc(sizeof(Conf));

    char *array[128] = { NULL };
    
    // parser yaml text
    assert(yaml_parser_initialize(&parser));
    yaml_parser_set_input_file(&parser, file);

    do {
        if (!yaml_parser_parse(&parser, &event)) {
            goto error;
        }
        if (!consume_event(&state, &event)) {
            goto error;
        }
        if (state.accepted && i < sizeof(data)/sizeof(*data)) {
            data[i].name = state.data.name;
            data[i].color = state.data.color;
            data[i].count = state.data.count;
            printf("data[%d]={name=%s, color=%s, count=%d}\n",
                i, data[i].name, data[i].color, data[i].count);
            i++;
        }
        yaml_event_delete(&event);
    } while (state.state != STOP);

    yaml_parser_delete(&parser);
    yaml_token_delete(&token);
    fclose(file);
    return 0;

error:
    yaml_parser_delete(&parser);
    yaml_token_delete(&token);
    fclose(file);
    return 1;

    return 0;
}