#ifndef URN_URN_H
#define URN_URN_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct urn_slice {
    const char *data;
    size_t len;
} urn_slice;

typedef struct urn {
    urn_slice nid;
    urn_slice nss;
    urn_slice r_component;
    urn_slice q_component;
    urn_slice f_component;
} urn;

bool urn_parse(const char *input, urn *out);
size_t urn_render(const urn *value, char *buffer, size_t buffer_len);

#ifdef __cplusplus
}
#endif

#endif
