/*
 * A368283  Expansion of e.g.f. exp(2*x) / (1 + log(1 - x)).
 * https://oeis.org/A368283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A368283
    Recurrence:  a(0) = 1
                 a(n) = 2^n + Σ_{k=1..n} (k-1)! · C(n,k) · a(n-k)
*/

#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

static seq_t *seq_cache = NULL;  /* array a[0] .. a[max_computed]   */
static idx_t  cache_len = 0;     /* number of stored terms           */
static seq_t  pow2_val  = 1;     /* 2^i for the current i           */

/* Compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* initialise empty cache for a(0) */
    if (seq_cache == NULL) {
        seq_cache = alloc_seq(1);   /* a[0] only */
        seq_cache[0] = 1;          /* a(0) = 1 */
        cache_len = 1;
        pow2_val = 1;              /* 2^0 = 1 */
    }

    /* extend cache up to n (if required) */
    if (n >= cache_len) {
        /* allocate new larger array */
        seq_t *new_cache = alloc_seq(n + 1);
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = seq_cache[i];
        free_seq(seq_cache);
        seq_cache = new_cache;

        /* compute unfinished terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* term 2^i */
            pow2_val = pow2_val * 2;   /* now pow2_val = 2^i */

            /* sum part: Σ_{k=1..i} (k-1)! * C(i,k) * a(i-k) */
            seq_t sum = 0;
            seq_t f   = 1;   /* (k-1)! for k=1  (0! = 1)          */
            seq_t bin = (seq_t)i; /* C(i,1) = i                      */

            for (idx_t k = 1; k <= i; ++k) {
                /* term = (k-1)! * C(i,k) * a(i-k) */
                seq_t term = f * bin;
                term = term * seq_cache[i - k];
                sum = sum + term;

                /* update for next k+1 (if k < i) */
                if (k < i) {
                    /* next factorial: (k)! = (k-1)! * k */
                    f = f * (seq_t)k;

                    /* next binomial: C(i,k+1) = C(i,k) * (i-k) / (k+1) */
                    seq_t numerator   = bin * (seq_t)(i - k);
                    seq_t denominator = (seq_t)(k + 1);
                    bin = numerator / denominator;
                }
            }

            /* a(i) = 2^i + sum */
            seq_cache[i] = pow2_val + sum;
        }

        cache_len = n + 1;
    }

    return seq_cache[n];
}
