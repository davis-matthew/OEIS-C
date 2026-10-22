/*
 * A331821  Positive numbers k such that -k and -(k + 1) are both
 *          negabinary-Niven numbers (A331728).
 * https://oeis.org/A331821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1987, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   negabinary_digit_sum_signed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n-th positive integer k such that -k and -(k+1)
   are negabinary‑Niven numbers (definition from OEIS A331821).  The
   function searches candidates in increasing order and caches the
   results. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/negabinary_digit_sum_signed.h"

/* --------------------------------------------------------------------
   Test whether a given k satisfies the definition of A331821.
   -------------------------------------------------------------------- */
static bool is_candidate(uint64_t k)
{
    uint64_t sum1 = negabinary_digit_sum_signed(-(int64_t)k);
    if (sum1 == 0 || k % sum1 != 0) return false;

    uint64_t sum2 = negabinary_digit_sum_signed(-(int64_t)(k + 1));
    if (sum2 == 0) return false;
    return ((k + 1) % sum2 == 0);
}

/* --------------------------------------------------------------------
   Dynamic array to cache computed terms.
   -------------------------------------------------------------------- */
static seq_t *cache      = NULL;
static size_t  cache_len = 0;          /* number of terms already stored */
static size_t  cache_cap = 0;          /* allocated capacity              */
static uint64_t next_k  = 1;           /* next candidate k to test        */

/* --------------------------------------------------------------------
   Grow the cache array to hold at least one more element.
   -------------------------------------------------------------------- */
static void cache_ensure(void)
{
    if (cache_len < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                  /* memory exhaustion */
    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------
   Return the n-th term of the sequence (n ≥ 1).
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n=0 */

    /* Compute until we have at least n terms */
    while (cache_len < (size_t)n) {
        if (is_candidate(next_k)) {
            cache_ensure();
            cache[cache_len++] = (seq_t)next_k;
        }
        next_k++;
    }
    return cache[n - 1];
}
