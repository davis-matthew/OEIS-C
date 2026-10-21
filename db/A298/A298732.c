/*
 * A298732  Number of compositions (ordered partitions) of n into parts > 1
 *          such that no two adjacent parts are equal (Carlitz compositions).
 * https://oeis.org/A298732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 55, u64 n <= 107,
 *            u128 n <= 212, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1,
 *          For n ≥ 1: a(n) = Σ_{k=1..n} s_k · a(n−k)
 *          where s_k = Σ_{d|k, d≥2} (−1)^{k/d−1}.
 *
 * This implements the recurrence derived from the generating‑function
 * 1/(1−∑_{k≥2} x^k/(1+x^k)).
 *
 * The arrays a_cache and s_cache are grown lazily to accommodate the
 * largest n requested.  The recurrence is evaluated in O(n²) time,
 * which is fast enough for n up to about 10 000 as required.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_small.h"

/* static memoisation tables */
static seq_t *a_cache = NULL;   /* a_cache[i] = a(i) */
static int   *s_cache = NULL;   /* s_cache[i] = s_i   */
static idx_t  cache_max = 0;    /* largest n computed so far */
static size_t cache_cap = 0;    /* current capacity of the tables */

/* Helper: compute the auxiliary sequence s up to n (inclusive) */
static void compute_s_cache(size_t up_to)
{
    size_t i;
    /* reset all entries to zero */
    for (i = 0; i < up_to; ++i)
        s_cache[i] = 0;

    for (size_t d = 2; d < up_to; ++d) {
        for (size_t m = d; m < up_to; m += d) {
            size_t j = m / d;            /* m = j·d  */
            int    sign = (j & 1) ? +1 : -1;
            s_cache[m] += sign;
        }
    }
}

/* Main sequence function as required by the test harness */
seq_t a(idx_t n)
{
    /* Special case for the offset */
    if (n == 0)
        return (seq_t)1;

    /* Extend storage if needed */
    size_t req_cap = (size_t)n + 1;
    if (cache_cap < req_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < req_cap) new_cap <<= 1;     /* double size */

        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        s_cache = (int   *)realloc(s_cache, new_cap * sizeof(int   ));
        cache_cap = new_cap;
    }

    /* Ensure a_cache[0] is initialised */
    if (cache_max == 0) {
        a_cache[0] = (seq_t)1;
    }

    /* Re‑compute s_cache if we increased the capacity */
    if (cache_cap > (size_t)(cache_max + 1)) {
        compute_s_cache(cache_cap);
    }

    /* Compute a values up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t k = 1; k <= i; ++k) {
            int sk = s_cache[(size_t)k];
            if (sk == 0) continue;

            seq_t prod = mul_small(a_cache[i - k],
                                   (unsigned int)(sk > 0 ? sk : -sk));

            if (sk > 0)
                pos += prod;
            else
                neg += prod;
        }
        a_cache[i] = pos - neg;   /* pos ≥ neg for all i */
    }
    cache_max = n;

    return a_cache[n];
}
