#include "alloc-inl.h"
#include "util.h"

#include <assert.h>

int consume_event(struct parser_state *s, yaml_event_t *event)
{
    s->accepted = 0;
    fprintf(stderr, "state: %d, ", s->state);
    fprintf(stderr, "event: %d, ",event->type);
    switch (s->state) {
    case START:
        switch (event->type) {
        case YAML_MAPPING_START_EVENT:
            s->state = ACCEPT_SECTION;
            break;
        case YAML_SCALAR_EVENT:
            fprintf(stderr, "Ignoring unexpected scalar: %s\n",
                    (char*)event->data.scalar.value);
            break;
        case YAML_SEQUENCE_START_EVENT:
            fprintf(stderr, "Unexpected sequence.\n");
            s->state = ERROR;
            break;
        case YAML_STREAM_END_EVENT: s->state = STOP; break;
        default:
            break;
        }
        break;
    case ACCEPT_SECTION:
        switch (event->type) {
        case YAML_SCALAR_EVENT:
            printf("scalar value: %s, ", (char*)event->data.scalar.value);
            if (strcmp((char*)event->data.scalar.value, "version") == 0) {
               s->state = ACCEPT_VERSION;
               s->cur_section = ACCEPT_VERSION;
            } 
            else if (strcmp((char*)event->data.scalar.value, "info") == 0) {
               s->state = ACCEPT_INFO;
               s->cur_section = ACCEPT_INFO;
            }
            else if (strcmp((char*)event->data.scalar.value, "fuzzer") == 0) {
               s->state = ACCEPT_FUZZER;
               s->cur_section = ACCEPT_FUZZER;
            }            
            else {
               fprintf(stderr, "Unexpected scalar: %s\n",
                      (char*)event->data.scalar.value);
               s->state = ERROR;
            }
            break;
        default:
            fprintf(stderr, "Unexpected event while getting scalar: %d\n", event->type);
            s->state = ERROR;
            break;
        }
        break;
    case ACCEPT_VERSION:
        switch (event->type) {
        case YAML_SCALAR_EVENT:
            s->value = (char*)event->data.scalar.value;
            s->data.version = strdup((char*)s->value);
            s->accepted = 1;
            s->state = ACCEPT_SECTION;            
            break;
        default:
            fprintf(stderr, "Unexpected event while getting version: %d\n", event->type);
            s->state = ERROR;
            break;            
        }        
        break;
    case ACCEPT_INFO:
        switch (event->type) {
        case YAML_MAPPING_START_EVENT: 
            memset(&(s->data.iConf), 0, sizeof(s->data.iConf));
            s->state = ACCEPT_KEY;
            break;
        default:
            fprintf(stderr, "Unexpected event while getting info sequence: %d\n", event->type);
            s->state = ERROR;
            break;
        }
        break;
    case ACCEPT_FUZZER:
        switch (event->type) {
        case YAML_MAPPING_START_EVENT: 
            memset(&(s->data.fConf), 0, sizeof(s->data.fConf));
            s->state = ACCEPT_KEY;
            break;
        default:
            fprintf(stderr, "Unexpected event while getting fuzzer sequence: %d\n", event->type);
            s->state = ERROR;
            break;
        }
        break;            
    case ACCEPT_LIST:
        switch (event->type) {
        case YAML_SEQUENCE_START_EVENT: s->state = ACCEPT_VALUES; break;
        case YAML_SCALAR_EVENT: s->state = ACCEPT_VALUES; break; // add by setsal
        default:
            fprintf(stderr, "Unexpected event while getting sequence: %d\n", event->type);
            s->state = ERROR;
            break;
        }
        break;
    case ACCEPT_VALUES:
        switch (event->type) {
        case YAML_SCALAR_EVENT:
            s->state = ACCEPT_KEY;
            break;
        case YAML_MAPPING_START_EVENT:
            memset(&(s->data), 0, sizeof(s->data));
            s->state = ACCEPT_KEY;
            break;
        case YAML_SEQUENCE_END_EVENT: s->state = START; break;
        case YAML_DOCUMENT_END_EVENT: s->state = START; break;
        default:
            fprintf(stderr, "Unexpected event while getting mapped values: %d\n",
                    event->type);
            s->state = ERROR;
            break;
        }
        break;
    case ACCEPT_KEY:
        switch (event->type) {
        case YAML_SCALAR_EVENT:
            s->key = strdup((char*)event->data.scalar.value);
            s->state = ACCEPT_VALUE;
            printf("key: %s, ", s->key);
            break;
        case YAML_MAPPING_END_EVENT:
            s->accepted = 1;
            s->state = ACCEPT_SECTION;
            break;
        default:
            fprintf(stderr, "Unexpected event while getting key: %d\n",
                    event->type);
            s->state = ERROR;
            break;
        }
        break;
    case ACCEPT_VALUE:
        switch (event->type) {       
        case YAML_SCALAR_EVENT:
            s->value = (char*)event->data.scalar.value;
            if (strcmp(s->key, "description") == 0) {
                s->data.iConf.description = strdup((char*)s->value);
            }
            else if (strcmp(s->key, "version") == 0) {
                s->data.iConf.version = strdup((char*)s->value);
            }
            else if (strcmp(s->key, "title") == 0) {
                s->data.iConf.title = strdup((char*)s->value);
            }
            else if (strcmp(s->key, "info_protocol") == 0) {
                s->data.iConf.protocol = strdup((char*)s->value);
            }
            else if (strcmp(s->key, "dictionary") == 0) {
                s->data.fConf.dictionary = strdup((char*)s->value);
            }
            else if (strcmp(s->key, "dirty_mode") == 0) {
                s->data.fConf.dirty_mode = (strcmp(s->value, "true") == 0) ? 1 : 0;
            }
            else if (strcmp(s->key, "fuzzer_protocol") == 0) {
                s->data.fConf.protocol = strdup((char*)s->value);
            }
            else if (strcmp(s->key, "ip_address") == 0) {
                s->data.fConf.ip_address = strdup((char*)s->value);
            }
            else if (strcmp(s->key, "port") == 0) {
                s->data.fConf.port = atoi(s->value);
            }
            else if (strcmp(s->key, "init_waitTime") == 0) {
                s->data.fConf.init_waitTime = atoi(s->value);
            }
            else if (strcmp(s->key, "state_selection_algo") == 0) {
                s->data.fConf.state_selection_algo = atoi(s->value);
            }            
            else if (strcmp(s->key, "seed_selection_algo") == 0) {
                s->data.fConf.seed_selection_algo = atoi(s->value);
            }
            else if (strcmp(s->key, "region_level_mutation") == 0) {
                s->data.fConf.region_level_mutation = (strcmp(s->value, "true") == 0) ? 1 : 0;
            }
            else if (strcmp(s->key, "state_aware_mode") == 0) {
                s->data.fConf.state_aware_mode = (strcmp(s->value, "true") == 0) ? 1 : 0;
            }
            else if (strcmp(s->key, "terminate_child") == 0) {
                s->data.fConf.terminate_child = (strcmp(s->value, "true") == 0) ? 1 : 0;
            }            
            else {
                fprintf(stderr, "Ignoring unknown key: %s\n", s->key);
            }
            free(s->key);
            s->state = ACCEPT_KEY;
            break;
        default:
            fprintf(stderr, "Unexpected event while getting value: %d\n",
                    event->type);
            s->state = ERROR;
            break;
        }
        break;
    case ERROR:
    case STOP:
        break;
    }
    return (s->state == ERROR ? 0 : 1);
}

// parse yaml
int parse_yaml_config(Conf *conf, char *path) {

    // read yaml file
    FILE *file = fopen(path, "r");
    assert(file);

    yaml_parser_t parser;
    yaml_event_t event;
    
    // parser yaml text
    assert(yaml_parser_initialize(&parser));
    yaml_parser_set_input_file(&parser, file);


    // malloc yaml
    struct parser_state state = {.state=START, .accepted=0, .error=0};

    do {
        if (!yaml_parser_parse(&parser, &event)) {
            goto error;
        }
        if (!consume_event(&state, &event)) {
            goto error;
        }
        if (state.accepted) {
            switch(state.cur_section) {
                case ACCEPT_VERSION:
                    conf->version = state.data.version;
                break;
                case ACCEPT_INFO:
                    conf->iConf.description = state.data.iConf.description;
                    conf->iConf.version = state.data.iConf.version;
                    conf->iConf.title = state.data.iConf.title;
                    conf->iConf.protocol = state.data.iConf.protocol;                
                break;
                case ACCEPT_FUZZER:
                    conf->fConf.dictionary = state.data.fConf.dictionary;
                    conf->fConf.dirty_mode = state.data.fConf.dirty_mode;
                    conf->fConf.protocol = state.data.fConf.protocol;
                    conf->fConf.ip_address = state.data.fConf.ip_address;
                    conf->fConf.port = state.data.fConf.port;
                    conf->fConf.init_waitTime = state.data.fConf.init_waitTime;
                    conf->fConf.state_selection_algo = state.data.fConf.state_selection_algo;
                    conf->fConf.region_level_mutation = state.data.fConf.region_level_mutation;
                    conf->fConf.state_aware_mode = state.data.fConf.state_aware_mode;
                    conf->fConf.terminate_child = state.data.fConf.terminate_child;
                break;
                default:
                break;
            }
        }
        printf("\n");
        yaml_event_delete(&event);
    } while (state.state != STOP);

    yaml_parser_delete(&parser);
    return 0;

error:
    yaml_parser_delete(&parser);
    return 1;
}