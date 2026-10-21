/*
 * A244510  a(0)=0; for n>0, choose a(n) to be the smallest number > a(n-1)
 *          such that the condition a(n) > Sum_{k=0..n} digsum(a(k)) holds.
 * https://oeis.org/A244510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 4709, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n>0, a(n) is the smallest integer > a(n‑1)
   satisfying a(n) > Σ_{k=0..n} digsum(a(k))  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

/* static state used for memoization */
static seq_t *seq_cache      = NULL;   /* array of computed terms      */
static idx_t  seq_cache_size = 0;      /* allocated capacity (seq_t*s) */
static idx_t  seq_cache_len  = 0;      /* number of terms stored       */
static seq_t  seq_sum_digsum = 0;      /* Σ_{k=0..m-1} digsum(a(k))   */

/* ensure a(n) is stored in *seq_cache[n] and seq_sum_digsum is updated */
static void seq_ensure_cached(idx_t n)
{
    if (seq_cache == NULL) {
        seq_cache_size = 1;
        seq_cache = (seq_t *)malloc(seq_cache_size * sizeof(seq_t));
        if (!seq_cache) exit(EXIT_FAILURE);
        seq_cache[0] = (seq_t)0;                     /* a(0) = 0             */
        seq_sum_digsum = digit_sum(seq_cache[0]);    /* = 0                   */
        seq_cache_len  = 1;
    }
    while (seq_cache_len <= n) {
        /* grow capacity if necessary */
        if (seq_cache_len >= seq_cache_size) {
            seq_cache_size *= 2;
            seq_t *tmp = (seq_t *)realloc(seq_cache,
                                          seq_cache_size * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            seq_cache = tmp;
        }

        seq_t prev     = seq_cache[seq_cache_len - 1];
        seq_t candidate = prev + 1;
        for (;;) {
            seq_t ds = digit_sum(candidate);
            /* candidate > (sum of previous digsum) + ds ? */
            if (candidate > seq_sum_digsum + ds) {
                seq_cache[seq_cache_len] = candidate;
                seq_sum_digsum += ds;   /* update Σ digsum          */
                break;
            }
            candidate++;               /* try next integer         */
        }
        seq_cache_len++;
    }
}

seq_t a(idx_t n)
{
    seq_ensure_cached(n);
    return seq_cache[n];
}
