/*
 * A363542  G.f. satisfies A(x) = exp( Sum_{k>=1} (-1)^(k+1) * (2^k + A(x^k))
 *          * x^k/k ).
 * https://oeis.org/A363542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n) = (-1/n) * sum_{k=1..n} ((-2)^k + sum_{d|k}
 *            (-1)^{k/d} * d * a(d-1)) * a(n-k) (equivalent to a(n) = (1/n) *
 *            sum_{k=1..n} k*L_k*a(n-k) with L_k defined above)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static seq_t *a_cache   = NULL;   /* a values */
static seq_t *val_pos   = NULL;   /* positive part of k*L_k */
static seq_t *val_neg   = NULL;   /* negative part of k*L_k */
static size_t cache_size = 0;     /* number of values computed (>0 includes a[0]) */

static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;                /* already computed */

    /* grow cache to accommodate n */
    size_t new_cap = (cache_size == 0) ? 1 : cache_size;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    if (cache_size == 0) {
        a_cache   = (seq_t*)malloc(new_cap * sizeof(seq_t));
        val_pos   = (seq_t*)malloc(new_cap * sizeof(seq_t));
        val_neg   = (seq_t*)malloc(new_cap * sizeof(seq_t));
    } else {
        a_cache   = (seq_t*)realloc(a_cache   , new_cap * sizeof(seq_t));
        val_pos   = (seq_t*)realloc(val_pos   , new_cap * sizeof(seq_t));
        val_neg   = (seq_t*)realloc(val_neg   , new_cap * sizeof(seq_t));
    }

    /* initialize for the new region */
    for (size_t i = cache_size; i < new_cap; ++i) {
        a_cache[i] = (seq_t)0;
        val_pos[i] = (seq_t)0;
        val_neg[i] = (seq_t)0;
    }

    /* compute terms from current cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;
            val_pos[0] = val_neg[0] = 0;
            continue;
        }

        /* --- compute k*L_k parts (positive and negative separately) --- */
        seq_t pos_val = 0;
        seq_t neg_val = 0;

        /* first term from (-1)^{k+1} 2^k */
        seq_t powk = pow2(i);
        if ((i & 1) == 0) {            /* even k => negative */
            neg_val += powk;
        } else {                       /* odd k => positive */
            pos_val += powk;
        }

        /* sum over divisors m of k */
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                /* divisor m1 = d */
                idx_t m1 = d;
                idx_t e1 = i / m1;                  /* k/m1 */
                seq_t term = a_cache[e1 - 1] * (seq_t)e1;
                if ((m1 & 1) == 0) neg_val += term;
                else             pos_val += term;

                /* divisor m2 = i/d (if distinct) */
                if (d != i / d) {
                    idx_t m2 = i / d;
                    idx_t e2 = i / m2;              /* which equals d */
                    seq_t term2 = a_cache[e2 - 1] * (seq_t)e2;
                    if ((m2 & 1) == 0) neg_val += term2;
                    else             pos_val += term2;
                }
            }
        }

        val_pos[i] = pos_val;
        val_neg[i] = neg_val;

        /* --- compute a[i] using convolution with previous values --- */
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t prod_pos = a_cache[i - k] * val_pos[k];
            seq_t prod_neg = a_cache[i - k] * val_neg[k];
            sum_pos += prod_pos;
            sum_neg += prod_neg;
        }
        seq_t diff = sum_pos - sum_neg;      /* guaranteed non‑negative */
        a_cache[i] = diff / (seq_t)i;        /* exact division */
    }

    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
