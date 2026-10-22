/*
 * A335575  Numbers k such that A000217(k)^A000217(k+1) mod A000217(k+2) is a
 *          triangular number.
 * https://oeis.org/A335575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k such that Tr(T_k)^Tr(T_{k+1}) mod T_{k+2}
 *            is a triangular number, where Tr(m)=m(m+1)/2. The sequence
 *            enumerates all such k in increasing order
 * Range:     u8 n <= 1, u16 n <= 34, u32 n <= 2466, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utilities and standard headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/floor_sqrt.h"

/* Helper: test whether a seq_t value is a triangular number. */
static bool
is_triangular(seq_t x)
{
    /* d = 8*x + 1 must be a perfect square */
    seq_t d = 8 * x + 1;
    seq_t sq = floor_sqrt(d);
    if (sq * sq != d)
        return false;
    /* m = (sqrt(d) - 1)/2 */
    seq_t m = (sq - 1) / 2;
    /* Check m(m+1)/2 == x */
    seq_t tri = m * (m + 1) / 2;
    return tri == x;
}

/* Helper: test whether a given k satisfies the definition. */
static bool
k_is_valid(idx_t k)
{
    seq_t kseq = (seq_t)k;

    /* Triangular numbers T_k, T_{k+1}, T_{k+2} */
    seq_t t1 = kseq * (kseq + 1) / 2;
    seq_t t2 = (kseq + 1) * (kseq + 2) / 2;
    seq_t t3 = (kseq + 2) * (kseq + 3) / 2;

    /* Compute t1^t2 mod t3 */
    /* t2 fits in uint64_t for the ranges of interest (k up to ~10^6). */
    seq_t res = mod_pow(t1, (uint64_t)(t2), t3);

    return is_triangular(res);
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    /* Static cache of previously found valid k's */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t next_k      = 1;    /* next k to test */

    /* Grow cache until we have at least n elements */
    while (cache_len < n) {
        /* Find next valid k */
        for (;;) {
            if (k_is_valid(next_k)) {
                /* Ensure capacity in cache */
                if (cache_cap <= cache_len) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_k;
                ++next_k;
                break;
            }
            ++next_k;
        }
    }

    /* Return the nth element (1‑based index) */
    return cache[n - 1];
}
