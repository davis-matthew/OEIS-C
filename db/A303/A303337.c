/*
 * A303337  Number of rectangles with semiprime area and dimensions (p) X
 *          (p+q) such that n = p+q, p < q.
 * https://oeis.org/A303337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} [Omega(n*i) = 2]
 * Range:     u8 n <= 3250, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _init_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where Omega(m) is the total number of prime factors of m counted with multiplicity. */
/* This implementation precomputes smallest prime factors up to the maximum needed product
   and counts Omega on the fly for each term. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/_init_spf.h"

/* Computes a single term a(n) as defined. */
seq_t a(idx_t n)
{
    /* Static data */
    static uint32_t *spf  = NULL;
    static size_t spf_limit = 0;

    /* Determine the maximum product needed for this n */
    size_t max_prod = 0;
    if (n > 1) {
        /* n*(n-1)/2 fits in 64-bit for n up to ~1e9 */
        max_prod = (size_t)n * (size_t)(n - 1) / 2;
    }

    /* Reinitialize sieve if necessary */
    if (max_prod > spf_limit) {
        if (spf) free(spf);
        _init_spf(max_prod, &spf, &spf_limit);
    }

    seq_t count = 0;
    if (n > 1) {
        idx_t half = (n - 1) / 2;
        for (idx_t i = 1; i <= half; ++i) {
            uint64_t val = (uint64_t)n * (uint64_t)i;
            int om = 0;
            while (val > 1) {
                uint32_t p = spf[val];
                val /= p;
                om++;
            }
            if (om == 2) count++;
        }
    }
    return count;
}
