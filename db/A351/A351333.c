/*
 * A351333  a(n) = Sum_{k=0..n} k! * (-k)^k * Stirling1(n,k).
 * https://oeis.org/A351333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} k! * (-k)^k * Stirling1(n,k)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_pow.h"

/* Ensure a dynamic array of seq_t has at least ‘needed’ elements. */
static void ensure_cap(size_t needed, size_t *cap, seq_t **ptr) {
    if (needed > *cap) {
        seq_t *p = (seq_t*)realloc(*ptr, needed * sizeof(seq_t));
        *ptr = p;
        *cap = needed;
    }
}

/* A351333 */
seq_t a(idx_t n) {
    /* Persistent state across calls */
    static idx_t          max_n   = 0;    /* highest n computed so far   */
    static size_t         a_cap   = 0;    /* capacity of a_cache          */
    static seq_t *        a_cache = NULL; /* cache of a(n) values          */
    static size_t         T_cap   = 0;    /* capacity of T_cache          */
    static seq_t *        T_cache = NULL; /* cache of k! * (-k)^k          */
    static idx_t          fact_k  = 0;    /* largest k for which fact_k!   */
    static seq_t          fact    = 1;    /* fact_k!                      */
    static size_t         s_cap_prev = 0; /* capacity of previous Stirling row */
    static seq_t *        s_prev = NULL; /* previous Stirling row (n-1)    */

    /* Initialise caches on the very first call */
    if (a_cache == NULL) {
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        a_cap = 1;

        T_cache = (seq_t*)malloc(sizeof(seq_t));
        T_cache[0] = (seq_t)1;
        T_cap = 1;

        s_prev = (seq_t*)malloc(sizeof(seq_t));
        s_prev[0] = (seq_t)1;
        s_cap_prev = 1;
    }

    /* Return cached value if already computed */
    if (n <= max_n) {
        return a_cache[n];
    }

    /* Extend T_cache to contain values up to n */
    ensure_cap((size_t)n + 1, &T_cap, &T_cache);
    if (fact_k < n) {
        for (idx_t k = fact_k + 1; k <= n; ++k) {
            fact = fact * (seq_t)k;                                 /* k!   */
            seq_t pow_k = seq_pow((seq_t)(-(seq_t)k), k);           /* (-k)^k */
            T_cache[k] = fact * pow_k;                             /* k! * (-k)^k */
        }
        fact_k = n;
    }

    /* Extend a_cache to hold results up to n */
    ensure_cap((size_t)n + 1, &a_cap, &a_cache);

    /* Compute Stirling rows from max_n+1 up to n */
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        size_t row_len = (size_t)cur + 1;
        seq_t *s_curr = (seq_t*)malloc(row_len * sizeof(seq_t));
        s_curr[0] = (seq_t)0; /* s(cur,0) = 0 for cur > 0 */

        for (idx_t k = 1; k <= cur; ++k) {
            seq_t term1 = s_prev[k - 1];
            seq_t term2 = 0;
            if (k <= cur - 1) {
                term2 = s_prev[k];
            }
            s_curr[k] = term1 - (seq_t)(cur - 1) * term2; /* recurrence */
        }

        /* Sum to form a(cur) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= cur; ++k) {
            sum += s_curr[k] * T_cache[k];
        }
        a_cache[cur] = sum;

        /* Prepare for next iteration */
        free(s_prev);
        s_prev = s_curr;
        s_cap_prev = row_len;
        max_n = cur;
    }

    return a_cache[n];
}
