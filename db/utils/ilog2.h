/* ilog2: floor(log2(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ILOG2_H
#define OEISC_UTILS_ILOG2_H
#include <stddef.h>
#include <stdint.h>

/* compute floor(log2(v)) for a strictly positive v */
static unsigned ilog2(idx_t v)
{
    /* v > 0, so builtin is safe */
    return 63u - (unsigned)__builtin_clzll(v);
}

#endif
