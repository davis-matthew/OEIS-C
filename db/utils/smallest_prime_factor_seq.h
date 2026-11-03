/* smallest_prime_factor_seq: the smallest prime factor of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_SMALLEST_PRIME_FACTOR_SEQ_H
#define OEISC_UTILS_SMALLEST_PRIME_FACTOR_SEQ_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Smallest prime factor of a composite seq_t value.
   Returns the factor as seq_t. */
static seq_t smallest_prime_factor_seq(seq_t x)
{
    if ((x % (seq_t)2) == 0)
        return (seq_t)2;
    for (uint64_t p = 3; ; p += 2) {
        seq_t pp = (seq_t)p * (seq_t)p;
        if (pp > x)
            break;
        if ((x % (seq_t)p) == 0)
            return (seq_t)p;
    }
    /* x must be prime; return itself */
    return x;
}

#endif
