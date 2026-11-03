/* triangular: n(n+1)/2.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_TRIANGULAR_H
#define OEISC_UTILS_TRIANGULAR_H
#include <stdint.h>

static inline idx_t triangular(idx_t r)
{
    return (idx_t)(((__int128)r * (r + 1)) / 2);
}

#endif
