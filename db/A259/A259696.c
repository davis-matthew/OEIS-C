/*
 * A259696  a(n) = Sum_{k=1..n-1} k^8*sigma(k)*sigma(n-k).
 * https://oeis.org/A259696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} k^8 * sigma(k) * sigma(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 79, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sigma values: sigma_cache[0] unused, sigma_cache[1..max] valid */
static seq_t *sigma_cache = NULL; /* pointer to the array */
static idx_t  sigma_max   = 0;    /* highest index currently cached */
static size_t sigma_cap   = 0;    /* allocated capacity */

/* Recompute sigma[1..max] using a simple sieve */
static void recompute_sigma(idx_t max)
{
    /* Allocate or resize the buffer */
    if (sigma_cap < (size_t)(max + 1)) {
        if (sigma_cache != NULL) {
            free(sigma_cache);
        }
        sigma_cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)(max + 1));
        sigma_cap   = (size_t)(max + 1);
    }

    /* Initialise to zero */
    for (idx_t i = 0; i <= max; ++i) {
        sigma_cache[i] = (seq_t)0;
    }

    /* Sieve: add divisor d to every multiple of d */
    for (idx_t d = 1; d <= max; ++d) {
        seq_t d_seq = (seq_t)d;
        for (idx_t m = d; m <= max; m += d) {
            sigma_cache[m] += d_seq;
        }
    }

    sigma_max = max;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)0;
    }

    /* Ensure sigma up to n-1 are available */
    idx_t needed_max = n - 1;
    if (needed_max > sigma_max) {
        recompute_sigma(needed_max);
    }

    seq_t result = (seq_t)0;

    for (idx_t k = 1; k < n; ++k) {
        /* Compute k^8 */
        seq_t k_pow = (seq_t)1;
        seq_t k_seq = (seq_t)k;
        /* Multiply k_seq 8 times (since seq_t may not be an integer type that supports 8-bit shift) */
        for (int i = 0; i < 8; ++i) {
            k_pow *= k_seq;
        }

        /* term = k^8 * sigma(k) * sigma(n-k) */
        seq_t term = k_pow * sigma_cache[k];
        term *= sigma_cache[n - k];

        result += term;
    }

    return result;
}
