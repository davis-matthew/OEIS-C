/* totient_u64_to_seq: Euler's totient phi(n).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_TOTIENT_U64_TO_SEQ_H
#define OEISC_UTILS_TOTIENT_U64_TO_SEQ_H
#include <stdint.h>

/* Compute Euler's totient of an odd number n (n>0).  The routine works
   for any n that fits in a 64‑bit unsigned integer, which suffices for
   the range of n considered in this sequence. */
static seq_t totient_u64_to_seq(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = n;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            /* p is a prime factor; remove all powers */
            while (tmp % p == 0) tmp /= p;
            result -= result / p;
        }
    }
    if (tmp > 1) result -= result / tmp; /* last prime factor */
    return (seq_t)result;
}

#endif
