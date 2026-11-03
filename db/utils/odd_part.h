/* odd_part: n with its factors of 2 removed.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ODD_PART_H
#define OEISC_UTILS_ODD_PART_H
#include <stdbool.h>
#include <stdint.h>

static seq_t odd_part(seq_t x)
{
    /* Strip factors of two.  Works for any unsigned integer or arbitrary-
       precision integer type that supports &, >>, and ==. */
    while ((x & 1) == 0)
        x >>= 1;
    return x;
}

#endif
