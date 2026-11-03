/* is_triangular: whether n is a triangular number.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_TRIANGULAR_H
#define OEISC_UTILS_IS_TRIANGULAR_H
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* helper: test whether a small integer sum is triangular.
   Sum fits in 64 bits for all practical purposes (digit sum of a 64‑bit integer <= 9*20). */
static int is_triangular(unsigned long long sum)
{
    unsigned long long t = 8ULL * sum + 1ULL;   /* 8*sum+1 must fit in 64 bits */
    unsigned long long i = 0;
    /* naive loop is fine – t is tiny (≤ 1441 for a 64‑bit integer) */
    while (i * i <= t) {
        ++i;
    }
    unsigned long long r = i - 1;
    return (r * r == t) && (r & 1ULL);   /* r must be odd */
}

#endif
