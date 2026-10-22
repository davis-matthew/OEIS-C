/*
 * A357009  E.g.f. satisfies log(A(x)) = (exp(x) - 1)^2 * A(x).
 * https://oeis.org/A357009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (2*k)! * (k+1)^(k-1) *
 *            Stirling2(n,2*k)/k!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

/* static state used for memoisation */
static idx_t max_n_computed = (idx_t)(-1);          /* highest n for which cache is valid */
static seq_t *cache = NULL;                       /* cache of a(n) values      */
static size_t cache_cap = 0;                      /* allocated capacity of cache */

static seq_t *prev_row = NULL;                    /* Stirling numbers of order n-1 */
static seq_t *cur_row  = NULL;                    /* Stirling numbers of order n   */
static size_t stir_cap = 0;                       /* allocated capacity of Stirling rows */

static idx_t fpow_cap = 0;                        /* allocated capacity of f & pow tables */
static seq_t *f_array   = NULL;                   /* (2*k)! / k!                     */
static seq_t *pow_array = NULL;                   /* (k+1)^(k-1)                     */

/* Ensure that f_array and pow_array contain values up to k = up_to (inclusive)  */
static void ensure_fpow(idx_t up_to)
{
    if (fpow_cap > up_to)
        return;

    size_t new_cap = up_to + 1;
    f_array   = (seq_t *)realloc(f_array,   new_cap * sizeof(seq_t));
    pow_array = (seq_t *)realloc(pow_array, new_cap * sizeof(seq_t));

    for (idx_t k = fpow_cap; k <= up_to; ++k) {
        if (k == 0) {                     /* not really used, but harmless      */
            f_array[0]   = (seq_t)1;
            pow_array[0] = (seq_t)1;
        } else {
            /* f_k = f_{k-1} * (2k-1) * (2k) / k  */
            seq_t val = f_array[k-1];
            val = val * (seq_t)(2*k-1);
            val = val * (seq_t)(2*k);
            val = val / (seq_t)k;
            f_array[k] = val;

            /* pow_k = (k+1)^(k-1)                   */
            pow_array[k] = big_pow((seq_t)(k+1), (seq_t)(k-1));
        }
    }
    fpow_cap = new_cap;
}

/* Ensure that Stirling rows are allocated for row i (length i+1)      */
static void ensure_stir_cap(idx_t i)
{
    if (stir_cap > i)
        return;
    size_t new_cap = i + 1;
    prev_row = (seq_t *)realloc(prev_row, new_cap * sizeof(seq_t));
    cur_row  = (seq_t *)realloc(cur_row, new_cap * sizeof(seq_t));
    stir_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if (max_n_computed != (idx_t)(-1) && n <= max_n_computed)
        return cache[n];

    /* Initialise for the first call */
    if (max_n_computed == (idx_t)(-1)) {
        /* a(0) */
        ensure_stir_cap(0);
        prev_row[0] = (seq_t)1;                  /* Stirling(0,0) = 1            */
        ensure_fpow(0);                         /* trivial bootstrap             */

        if (cache == NULL || cache_cap == 0) {
            cache_cap = 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[0] = (seq_t)1;
        max_n_computed = 0;
    }

    /* Compute all required rows up to n */
    for (idx_t i = max_n_computed + 1; i <= n; ++i) {
        ensure_stir_cap(i);

        /* Compute Stirling numbers for row i */
        cur_row[0] = 0;
        for (idx_t m = 1; m <= i; ++m) {
            seq_t val = 0;
            if (m < i)          /* m * prev_row[m] is defined only when m <= i-1 */
                val += (seq_t)m * prev_row[m];
            val += prev_row[m-1];
            cur_row[m] = val;
        }

        /* Compute a(i) from its defining sum */
        idx_t kmax = i / 2;
        ensure_fpow(kmax);

        seq_t sum = 0;
        for (idx_t k = 1; k <= kmax; ++k) {
            idx_t idx = 2 * k;
            if (idx > i)
                continue;
            seq_t s = cur_row[idx];
            if (s == 0)
                continue;
            seq_t term = s * f_array[k];
            term *= pow_array[k];
            sum += term;
        }

        /* Store the result in the cache */
        if (i >= cache_cap) {
            cache_cap = i + 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[i] = sum;

        /* Prepare for next iteration: swap rows */
        seq_t *tmp = prev_row;
        prev_row  = cur_row;
        cur_row   = tmp;
    }

    max_n_computed = n;
    return cache[n];
}
