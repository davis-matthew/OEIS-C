/*
 * A243664  Number of 3-packed words of degree n.
 * https://oeis.org/A243664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(3*n,3*k) * a(n-k), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Hash in a client environment: seq_t, idx_t are provided externally. */

/* Private cache for memoisation. */
static seq_t *cache   = NULL;   /* array of computed a(n) values          */
static size_t cache_size = 0;   /* number of entries currently filled    */
static size_t cache_cap  = 0;   /* allocated capacity (in array elements) */

/* Grow the cache array to accommodate index n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Allocate / grow cache as required. */
    if ((size_t)n >= cache_size) {
        ensure_cache(n);
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }

            seq_t sum = 0;          /* running sum for a(i)               */
            seq_t bin = 1;          /* binomial(3*i, 3* (k-1))  initially 1 */

            for (size_t k = 1; k <= i; ++k) {
                /* Compute numerator and denominator for the
                   ratio binomial(3*i,3*k) / binomial(3*i,3*(k-1)) */
                idx_t mul1 = 3*(idx_t)i - 3*(k-1);          /* 3*i-3*(k-1)   */
                idx_t mul2 = mul1 - 1;                       /* -1             */
                idx_t mul3 = mul1 - 2;                       /* -2             */

                idx_t den1 = 3*k;                            /* 3*k            */
                idx_t den2 = den1 - 1;                       /* -1             */
                idx_t den3 = den1 - 2;                       /* -2             */

                seq_t num = (seq_t)mul1 * (seq_t)mul2 * (seq_t)mul3;
                seq_t den = (seq_t)den1 * (seq_t)den2 * (seq_t)den3;

                /* Update binomial value for current k. */
                bin = bin * num;
                bin = bin / den;        /* exact division */

                /* Accumulate term bin * a[i-k] into sum. */
                sum += bin * cache[i - k];
            }
            cache[i] = sum;
        }
        cache_size = (size_t)n + 1;
    }
    return cache[n];
}
