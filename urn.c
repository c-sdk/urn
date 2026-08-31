#include <urn.h>

#include <ctype.h>
#include <string.h>

static bool is_nid_char(char c)
{
    return isalnum((unsigned char)c) || c == '-';
}

bool urn_parse(const char *input, urn *out)
{
    if (input == NULL || out == NULL) {
        return false;
    }

    const size_t len = strlen(input);
    if (len < 7 || strncmp(input, "urn:", 4) != 0) {
        return false;
    }

    const char *nid = input + 4;
    const char *colon = strchr(nid, ':');
    if (colon == NULL) {
        return false;
    }

    const size_t nid_len = (size_t)(colon - nid);
    if (nid_len < 2 || nid_len > 32 || nid[0] == '-' || nid[nid_len - 1] == '-') {
        return false;
    }

    for (size_t i = 0; i < nid_len; i++) {
        if (!is_nid_char(nid[i])) {
            return false;
        }
    }

    const char *nss = colon + 1;
    if (*nss == '\0') {
        return false;
    }

    const char *r = strstr(nss, "?+");
    const char *q = strstr(nss, "?=");
    const char *f = strchr(nss, '#');
    const char *nss_end = input + len;

    if (r != NULL && r < nss_end) {
        nss_end = r;
    }
    if (q != NULL && q < nss_end) {
        nss_end = q;
    }
    if (f != NULL && f < nss_end) {
        nss_end = f;
    }
    if (nss_end == nss) {
        return false;
    }

    *out = (urn){
        .nid = { nid, nid_len },
        .nss = { nss, (size_t)(nss_end - nss) },
        .r_component = { NULL, 0 },
        .q_component = { NULL, 0 },
        .f_component = { NULL, 0 },
    };

    if (r != NULL) {
        const char *end = q != NULL && q > r ? q : f != NULL && f > r ? f : input + len;
        out->r_component = (urn_slice){ r + 2, (size_t)(end - (r + 2)) };
    }
    if (q != NULL) {
        const char *end = f != NULL && f > q ? f : input + len;
        out->q_component = (urn_slice){ q + 2, (size_t)(end - (q + 2)) };
    }
    if (f != NULL) {
        out->f_component = (urn_slice){ f + 1, (size_t)((input + len) - (f + 1)) };
    }

    return true;
}
