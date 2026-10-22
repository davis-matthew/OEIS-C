/*
 * A355787  E.g.f. satisfies A(x) = (1 + 2*x)^(A(x)/2).
 * https://oeis.org/A355787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * (k+1)^(k-1) * Stirling1(n,k)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where Stirling1(n,k) are signed Stirling numbers of the first kind.   */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

/* Cached data for successive calls. */
static seq_t *cache_a    = NULL;   /* a values                */
static seq_t *stirling   = NULL;   /* s(n,k) for current n   */
static seq_t *pow2       = NULL;   /* 2^i                    */
static seq_t *pow_k      = NULL;   /* (k+1)^(k-1)             */
static size_t cached_n   = 0;       /* highest n computed      */
static size_t allocated  = 0;       /* current allocated size  */

/* ------------------------------------------------------------------ */
/*  Ensure that we have space for indices 0..new_n (inclusive).
 *  new_n is the largest index we need to support.                    */
static void ensure_capacity(size_t new_n)
{
    if (new_n < allocated)
        return;

    /* Allocate new, zero‑initialized blocks */
    seq_t *new_a    = (seq_t *)calloc(new_n + 1, sizeof(seq_t));
    seq_t *new_s    = (seq_t *)calloc(new_n + 1, sizeof(seq_t));
    seq_t *new_p2   = (seq_t *)calloc(new_n + 1, sizeof(seq_t));
    seq_t *new_pk   = (seq_t *)calloc(new_n + 1, sizeof(seq_t));

    /* Copy old contents */
    if (cache_a)   memcpy(new_a,    cache_a,    allocated * sizeof(seq_t));
    if (stirling)  memcpy(new_s,   stirling,  allocated * sizeof(seq_t));
    if (pow2)      memcpy(new_p2,   pow2,      allocated * sizeof(seq_t));
    if (pow_k)     memcpy(new_pk,   pow_k,     allocated * sizeof(seq_t));

    /* Free old blocks */
    free(cache_a);
    free(stirling);
    free(pow2);
    free(pow_k);

    /* Install new arrays */
    cache_a    = new_a;
    stirling   = new_s;
    pow2       = new_p2;
    pow_k      = new_pk;
    allocated  = new_n + 1;
}

/* ------------------------------------------------------------------ */
/*  Sequence generator                                               */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* First call: initialise for n = 0 */
    if (!cache_a) {
        ensure_capacity(0);              /* allocate size 1 */

        cache_a[0]   = 1;                /* a(0) = 1       */
        stirling[0]  = 1;                /* s(0,0) = 1     */
        pow2[0]      = 1;                /* 2^0 = 1        */
        pow_k[0]     = 1;                /* (0+1)^(0-1)=1  */
        cached_n     = 0;
    }

    if (idx <= cached_n)
        return cache_a[idx];

    /* Extend all arrays up to idx */
    ensure_capacity(idx);

    /* Compute values for indices cached_n+1 .. idx */
    for (size_t cur = cached_n + 1; cur <= idx; ++cur) {

        /* 2^cur */
        pow2[cur] = pow2[cur - 1] * (seq_t)2;

        /* (cur+1)^(cur-1)  for cur > 0 */
        if (cur > 0)
            pow_k[cur] = big_pow((seq_t)(cur + 1), (seq_t)(cur - 1));
        else
            pow_k[cur] = 1;              /* cur == 0 handled already */

        /* Update Stirling numbers for s(cur,k)  (in‑place). */
        /* Use the recurrence: s(n,k) = s(n-1,k-1) - (n-1)*s(n-1,k). */
        for (size_t k = cur; k >= 1; --k) {
            seq_t prev = stirling[k];
            stirling[k] = stirling[k - 1] - (seq_t)(cur - 1) * prev;
        }
        stirling[0] = 0;                 /* s(cur,0) = 0 for cur>0 */

        /* Compute a(cur) = Σ_{k=0..cur} 2^(cur-k) * (k+1)^(k-1) * s(cur,k) */
        seq_t sum = 0;
        for (size_t k = 0; k <= cur; ++k) {
            seq_t term = pow2[cur - k];
            term *= pow_k[k];
            term *= stirling[k];
            sum += term;
        }
        cache_a[cur] = sum;
    }

    cached_n = idx;
    return cache_a[idx];
}
