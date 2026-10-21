/*
 * A271331  Rank 2 cluster algebra recurrence with parameters b=4 and c=3 and
 *          initial conditions x=2 and y=3.
 * https://oeis.org/A271331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2, a(1)=3;
   a(n) = (1 + a(n-1)^4)/a(n-2)  for even n,
   a(n) = (1 + a(n-1)^3)/a(n-2)  for odd n. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Static memoization array: cache[i] holds a(i). */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of elements stored */

/* Ensure the array is large enough for indices up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();   /* out‑of‑memory */
        cache = new_cache;
        cache_cap = new_cap;
    }

/* Compute any missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)2;
        } else if (i == 1) {
            val = (seq_t)3;
        } else {
            seq_t exp = (i % 2 == 0) ? (seq_t)4 : (seq_t)3;
            seq_t pow_val = big_pow(cache[i - 1], exp);
            val = (pow_val + (seq_t)1) / cache[i - 2];
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;   /* all terms up to n are now cached */
    return cache[n];
}
