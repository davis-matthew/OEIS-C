/*
 * A396208  a(n) = Sum_{i=0..n} Sum_{j=0..i} binomial(n+3*i,4*i) *
 *          binomial(i+3*j,4*j).
 * https://oeis.org/A396208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A396208
   a(n) = Σ_{i=0..n} Σ_{j=0..i} binomial(n+3*i,4*i) * binomial(i+3*j,4*j)

   Recurrence:  a(n) = 21*a(n-1) - 186*a(n-2) + 942*a(n-3) - 3105*a(n-4) 
                 + 7203*a(n-5) - 12404*a(n-6) + 16400*a(n-7) - 16998*a(n-8) 
                 + 13963*a(n-9) - 9114*a(n-10) + 4698*a(n-11) - 1880*a(n-12) 
                 + 565*a(n-13) - 120*a(n-14) + 16*a(n-15) - a(n-16)

   Initial values (a(0) … a(16)):
     1, 3, 18, 120, 819, 5622, 38580, 264344, 1809009, 12370698,
     84564501, 577978635, 3950105850, 26995878888, 184495083061,
     1260876796656, 8617102833670
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static const int coeffs[17] = {
    21, -186, 942, -3105, 7203,
    -12404, 16400, -16998, 13963, -9114,
    4698, -1880, 565, -120, 16, -1
};

/* Compute the first 17 terms from the double‑summing definition. */
static void compute_init_terms(seq_t *terms)
{
    for (idx_t n = 0; n < 17; ++n) {
        seq_t total = 0;
        for (idx_t i = 0; i <= n; ++i) {
            seq_t outer = binomial(n + 3 * i, 4 * i);
            seq_t inner = 0;
            for (idx_t j = 0; j <= i; ++j) {
                inner += binomial(i + 3 * j, 4 * j);
            }
            total += outer * inner;
        }
        terms[n] = total;
    }
}

/* Dynamic cache for computed values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;    /* numbers computed (valid indices 0 .. cache_len-1) */
static size_t cache_cap = 0;    /* allocated capacity */

/* Ensure the cache can hold at least indices 0..needed (inclusive). */
static void ensure_cache(idx_t needed)
{
    if ((size_t)needed < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= (size_t)needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_arr;
    cache_cap = new_cap;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    /* Initialise first 17 terms on the first call. */
    if (cache_len == 0) {
        ensure_cache(16);
        compute_init_terms(cache);
        cache_len = 17;
    }

    if ((size_t)n < cache_len)
        return cache[n];

    /* Extend cache up to n using the 16‑term recurrence. */
    ensure_cache(n);
    for (idx_t m = cache_len; m <= n; ++m) {
        seq_t val = 0;
        for (int k = 1; k <= 17; ++k) {
            seq_t prev = cache[m - k];
            int c = coeffs[k - 1];
            if (c >= 0)
                val += prev * (seq_t)c;
            else
                val -= prev * (seq_t)(-c);
        }
        cache[m] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
