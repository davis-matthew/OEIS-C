/* radical: the product of the distinct primes dividing n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_RADICAL_H
#define OEISC_UTILS_RADICAL_H
#include <stdint.h>

static seq_t radical(idx_t m)
{
    seq_t r = (seq_t)1;
    if (m < 2) return r;
    /* Handle factor 2 */
    if ((m & 1) == 0) {
        r *= (seq_t)2;
        while ((m & 1) == 0) m >>= 1;
    }
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            r *= (seq_t)p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) r *= (seq_t)m;  /* remaining prime factor */
    return r;
}

#endif
