#include "urn.h"

#include <string.h>
#include <traits.h>
#include <traits-unit.h>

static void assert_slice(urn_slice slice, const char *want)
{
    assert_equal(strlen(want), slice.len);
    assert_memory_equal(want, slice.data, slice.len);
}

Feature(parses_basic_urn)
{
    urn parsed;

    assert_true(urn_parse("urn:example:animal:ferret:nose", &parsed));
    assert_slice(parsed.nid, "example");
    assert_slice(parsed.nss, "animal:ferret:nose");
}

Feature(parses_r_q_and_f_components)
{
    urn parsed;

    assert_true(urn_parse("urn:example:a123?+abc?=xyz#frag", &parsed));
    assert_slice(parsed.nid, "example");
    assert_slice(parsed.nss, "a123");
    assert_slice(parsed.r_component, "abc");
    assert_slice(parsed.q_component, "xyz");
    assert_slice(parsed.f_component, "frag");
}

Feature(rejects_invalid_urns)
{
    urn parsed;

    assert_false(urn_parse("http://example.com", &parsed));
    assert_false(urn_parse("urn:x:nss", &parsed));
    assert_false(urn_parse("urn:-bad:nss", &parsed));
}

Feature(renders_basic_urn)
{
    urn value = {
        .nid = { "example", 7 },
        .nss = { "animal:ferret:nose", 18 },
    };
    char buffer[64];

    assert_equal(30, urn_render(&value, buffer, sizeof(buffer)));
    assert_string_equal("urn:example:animal:ferret:nose", buffer);
}

Feature(renders_r_q_and_f_components)
{
    urn value = {
        .nid = { "example", 7 },
        .nss = { "a123", 4 },
        .r_component = { "abc", 3 },
        .q_component = { "xyz", 3 },
        .f_component = { "frag", 4 },
    };
    char buffer[64];

    assert_equal(31, urn_render(&value, buffer, sizeof(buffer)));
    assert_string_equal("urn:example:a123?+abc?=xyz#frag", buffer);
}

Feature(reports_required_render_length_when_buffer_is_small)
{
    urn value = {
        .nid = { "example", 7 },
        .nss = { "animal:ferret:nose", 18 },
    };
    char buffer[8];

    assert_equal(30, urn_render(&value, buffer, sizeof(buffer)));
    assert_string_equal("urn:exa", buffer);
}

Describe("urn parser",
    Trait("parse",
        Run(parses_basic_urn),
        Run(parses_r_q_and_f_components),
        Run(rejects_invalid_urns)
    ),
    Trait("render",
        Run(renders_basic_urn),
        Run(renders_r_q_and_f_components),
        Run(reports_required_render_length_when_buffer_is_small)
    )
)
