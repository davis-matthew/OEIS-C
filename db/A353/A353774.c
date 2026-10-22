/*
 * A353774  Expansion of e.g.f. 1/(1 - (exp(x) - 1)^3).
 * https://oeis.org/A353774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = sum_{k=1..n} binom(n,k) * w[k] * a[n-k]
 *   where w[k] = 3^k - 3*2^k + 3
 * This is equivalent to the recurrence 
 *   a(n) = 6 * sum_{k=1..n} binom(n,k) * Stirling2(k,3) * a[n-k].
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *a_arr = NULL;            /* array holding a[0..max]    */
    static size_t a_cap = 0;               /* current capacity in slots   */
    static seq_t *w_arr = NULL;            /* array holding w[1..max]    */
    static size_t w_cap = 0;               /* capacity for w_arr          */
    static idx_t computed_max = 0;          /* largest n for which a is set*/
    static seq_t pow2_exp = 1;             /* current value of 2^k       */
    static seq_t pow3_exp = 1;             /* current value of 3^k       */

    /* First-time allocation: ensure a[0] = 1 and w[0] = 0 */
    if (!a_arr) {
        a_cap = 1;
        a_arr = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (!a_arr) abort();
        a_arr[0] = 1;

        w_cap = 1;
        w_arr = (seq_t *)malloc(w_cap * sizeof(seq_t));
        if (!w_arr) abort();
        w_arr[0] = 0;  /* unused, keeps indices aligned */
    }

    /* If n already computed, return immediately */
    if (n <= computed_max)
        return a_arr[n];

    /* Extend w_arr to hold entries up to n */
    size_t need_w = (size_t)n + 1;
    if (w_cap < need_w) {
        size_t new_w_cap = w_cap ? w_cap * 2 : 1;
        while (new_w_cap < need_w) new_w_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(w_arr, new_w_cap * sizeof(seq_t));
        if (!tmp) abort();
        w_arr = tmp;
        w_cap = new_w_cap;
    }
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        pow2_exp = pow2_exp * 2;           /* 2^k  */
        pow3_exp = pow3_exp * 3;           /* 3^k  */
        w_arr[k] = pow3_exp - 3 * pow2_exp + 3;
    }

    /* Extend a_arr to hold entries up to n */
    size_t need_a = (size_t)n + 1;
    if (a_cap < need_a) {
        size_t new_a_cap = a_cap ? a_cap * 2 : 1;
        while (new_a_cap < need_a) new_a_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_arr, new_a_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_arr = tmp;
        a_cap = new_a_cap;
    }

    /* Compute a values for all indices from computed_max+1 up to n */
    for (idx_t cur = computed_max + 1; cur <= n; ++cur) {
        seq_t sum = 0;
        seq_t binom = 1;  /* binom(cur,0) */
        for (idx_t k = 1; k <= cur; ++k) {
            binom = binom * (seq_t)(cur - k + 1) / (seq_t)k;   /* binom(cur,k) */
            seq_t term = binom * w_arr[k];
            term = term * a_arr[cur - k];
            sum += term;
        }
        a_arr[cur] = sum;
    }
    computed_max = n;
    return a_arr[n];
}
