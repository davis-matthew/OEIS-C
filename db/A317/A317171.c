/*
 * A317171  a(n) = n! * [x^n] 1/(1 + n*log(1 - x)).
 * https://oeis.org/A317171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} |Stirling1(n,k)| * n^k * k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   allocate_arr, resize_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/allocate_arr.h"
#include "../utils/resize_arr.h"

static void free_arr(seq_t* p) {
#ifdef __cplusplus
    delete [] p;
#else
    free(p);
#endif
}

/* Persistent caches.  They are initialised on first call. */
static seq_t *a_cache    = NULL;   /* a(n) values, 0..a_cache_len-1 */
static size_t a_cache_len = 0;      /* current number of cached values */

static seq_t *s_prev    = NULL;   /* Stirling1 numbers for previous n */
static size_t s_prev_len = 0;      /* length of s_prev (n+1) */

static seq_t *fact_arr  = NULL;   /* factorials k! up to fact_len-1 */
static size_t fact_len  = 0;       /* capacity of fact_arr */

/* Compute a(n).  Cached values are reused for any n. */
seq_t a(idx_t n)
{
    /* Initialise caches on first call. */
    if (a_cache == NULL) {
        /* a(0) = 1 */
        a_cache = allocate_arr(1);
        a_cache[0] = (seq_t)1;
        a_cache_len = 1;

        /* S(0,0) = 1 */
        s_prev = allocate_arr(1);
        s_prev[0] = (seq_t)1;
        s_prev_len = 1;

        /* 0! = 1 */
        fact_arr = allocate_arr(1);
        fact_arr[0] = (seq_t)1;
        fact_len = 1;
    }

    /* Expand caches up to required n. */
    while (a_cache_len <= (size_t)n) {
        idx_t next_n = (idx_t)a_cache_len;   /* compute value for n = next_n */

        /* Allocate new Stirling1 row: size next_n + 1. */
        seq_t *s_new = allocate_arr(next_n + 1);
        s_new[0] = (seq_t)0;  /* S(n,0) = 0 for n>0 */

        for (idx_t k = 1; k <= next_n; ++k) {
            seq_t term1 = (k - 1 < (idx_t)s_prev_len) ? s_prev[k - 1] : (seq_t)0;
            seq_t term2 = (k < (idx_t)s_prev_len)
                          ? (seq_t)(next_n - 1) * s_prev[k]
                          : (seq_t)0;
            s_new[k] = term1 + term2;
        }

        /* Expand factorial array if needed. */
        if (fact_len <= (size_t)next_n) {
            size_t old_cap = fact_len;
            fact_arr = resize_arr(fact_arr, old_cap, (size_t)next_n + 1);
            for (idx_t k = old_cap; k <= next_n; ++k) {
                if (k == 0)
                    fact_arr[0] = (seq_t)1;
                else
                    fact_arr[k] = fact_arr[k - 1] * (seq_t)k;
            }
            fact_len = (size_t)next_n + 1;
        }

        /* Compute a(next_n) = sum_{k=0..next_n} S(next_n,k)*next_n^k*k!. */
        seq_t sum = (seq_t)0;
        seq_t n_power = (seq_t)1;  /* next_n^0 */

        for (idx_t k = 0; k <= next_n; ++k) {
            seq_t term = s_new[k];
            term *= n_power;
            term *= fact_arr[k];
            sum += term;
            n_power *= (seq_t)next_n;
        }

        /* Store computed value in cache. */
        a_cache = resize_arr(a_cache, a_cache_len, a_cache_len + 1);
        a_cache[a_cache_len] = sum;
        ++a_cache_len;

        /* Prepare for next iteration. Replace previous Stirling row. */
        free_arr(s_prev);
        s_prev = s_new;
        s_prev_len = (size_t)next_n + 1;
    }

    return a_cache[n];
}
