#include "alloc-inl.h"
#include "util.h"

#include <yaml.h>

#define BUFFER_SIZE 65536
#define MAX_DOCUMENTS  16

int parse_yaml_config(u8 *filename) {

    FILE *file;
    yaml_parser_t parser;
    yaml_emitter_t emitter;

    yaml_document_t document;
    unsigned char buffer[BUFFER_SIZE+1];
    size_t written = 0;
    _document_t documents[MAX_DOCUMENTS];
    size_t document_number = 0;
    int done = 0;
    int count = 0;
    int error = 0;
    int k;

    memset(buffer, 0, BUFFER_SIZE+1);
    memset(documents, 0, MAX_DOCUMENTS*sizeof(yaml_document_t));
    
    printf("Loading, dumping, and loading again '%s': ", filename);
    fflush(stdout);
    
    file = fopen(filename, "rb");
    assert(file);
    assert(yaml_parser_initialize(&parser));
    yaml_parser_set_input_file(&parser, file);
    assert(yaml_emitter_initialize(&emitter));
    yaml_emitter_set_output_string(&emitter, buffer, BUFFER_SIZE, &written);
    yaml_emitter_open(&emitter);
    while (!done)
    {
        if (!yaml_parser_load(&parser, &document)) {
            error = 1;
            break;
        }
        done = (!yaml_document_get_root_node(&document));
        if (!done) {
            assert(document_number < MAX_DOCUMENTS);
            assert(copy_document(&(documents[document_number++]), &document));
            assert(yaml_emitter_dump(&emitter, &document) || 
                    (yaml_emitter_flush(&emitter) && print_output(filename, buffer, written, count)));
            count ++;
        }
        else {
            yaml_document_delete(&document);
        }
    }
    yaml_parser_delete(&parser);
    assert(!fclose(file));
    yaml_emitter_close(&emitter);
    yaml_emitter_delete(&emitter);
    if (!error)
    {
        count = done = 0;
        assert(yaml_parser_initialize(&parser));
        yaml_parser_set_input_string(&parser, buffer, written);
        while (!done)
        {
            assert(yaml_parser_load(&parser, &document) || print_output(argv[number], buffer, written, count));
            done = (!yaml_document_get_root_node(&document));
            if (!done) {
                assert(compare_documents(documents+count, &document) || print_output(argv[number], buffer, written, count));
                count ++;
            }
            yaml_document_delete(&document);
        }
        yaml_parser_delete(&parser);
    }
    for (k = 0; k < document_number; k ++) {
        yaml_document_delete(documents+k);
    }
    print_output(filename, buffer, written, -1);    
    return 0;
}