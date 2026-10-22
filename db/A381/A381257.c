/*
 * A381257  Numbers k such that 6*k+1 divides 6^k+1.
 * https://oeis.org/A381257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implements OEIS A381257: Numbers k such that 6*k+1 divides 6^k+1.
   a(1) = 0, a(2) = 1, a(3) = 6, ... 
   The function generates candidates incrementally and stores them in a
   static cache so that repeated calls with arbitrary n are fast.
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"   /* Provides mod_pow(seq_t base, idx_t exp, seq_t mod) */

static seq_t *cache = NULL;    /* dynamic array of found k values */
static size_t cache_len = 0;   /* number of filled entries in cache */
static idx_t next_k = 0;       /* next integer k to test */

/* Return the n-th element of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) {              /* offset 1: a(1) is the first term */
        return (seq_t)0;
    }

    /* Grow the cache until it contains at least n elements. */
    while (cache_len < n) {
        seq_t mod = (seq_t)6 * (seq_t)next_k + (seq_t)1;    /* 6*k+1 */
        seq_t pow = mod_pow((seq_t)6, next_k, mod);         /* 6^k mod (6*k+1) */
        seq_t one = (seq_t)1;
        seq_t mod_minus_one = mod - one;

        if (pow == mod_minus_one) {                         /* condition satisfied */
            /* realloc with explicit cast to avoid C++ conversion error */
            cache = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
            if (!cache) abort();                            /* allocation failed */
            cache[cache_len] = (seq_t)next_k;
            cache_len++;
        }
        next_k++;
    }

    return cache[n - 1];
}
