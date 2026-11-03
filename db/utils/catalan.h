/* catalan: the Catalan number C(n).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_CATALAN_H
#define OEISC_UTILS_CATALAN_H
#include <stdint.h>

/* The Catalan number C(n), exact whenever it fits in seq_t: each step computes
   C(i+1) = C(i) * (4i+2) / (i+2) as (c/g) * ((4i+2)/((i+2)/g)) with
   g = gcd(c mod (i+2), i+2), so no intermediate value exceeds C(i+1). */
static seq_t catalan(idx_t n)
{
    seq_t c = 1;
    for (idx_t i = 0; i < n; i++) {
        idx_t d = i + 2, g = d, x = (idx_t)(c % (seq_t)d);
        while (x != 0) {
            idx_t t = g % x;
            g = x;
            x = t;
        }
        c = (c / (seq_t)g) * (seq_t)((4 * i + 2) / (d / g));
    }
    return c;
}

#endif
