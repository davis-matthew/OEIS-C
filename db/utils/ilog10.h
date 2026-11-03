/* ilog10: floor(log10(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ILOG10_H
#define OEISC_UTILS_ILOG10_H
#include <stdint.h>

static inline seq_t ilog10(seq_t x)
{
    /* floor( log10( x ) ) : count decimal digits minus one */
    seq_t d = 0;
    while (x >= 10)
    {
        x /= 10;
        d++;
    }
    return d;
}

#endif
