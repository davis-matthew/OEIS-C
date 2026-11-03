/* totient_seq: Euler's totient phi(n).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_TOTIENT_SEQ_H
#define OEISC_UTILS_TOTIENT_SEQ_H
#include <stdint.h>

/* Compute Euler's totient of a 64‑bit integer.
   The values produced by A048701(n) for n ≤ 10000 fit in 64 bits,
   so a simple trial division is sufficient. */
static seq_t totient_seq(seq_t m)
{
    uint64_t mm = (uint64_t)m;
    uint64_t phi = mm;
    uint64_t t   = mm;

    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            phi -= phi / p;
        }
    }
    if (t > 1) phi -= phi / t;
    return (seq_t)phi;
}

#endif
