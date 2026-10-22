/*
 * A308647  a(n) = exp(1) * Sum_{k>=0} (-1)^k*k^(2*n+1)/k!.
 * https://oeis.org/A308647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..2*n+1} (-1)^k*Stirling2(2*n+1,k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value a(n) equals B_{2*n+1}(-1), the Bell polynomial evaluated at -1. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage for the sequence T(m) = Sum_{k=0..m} (-1)^k*Stirling2(m,k) */
    static seq_t *T = NULL;
    static size_t T_len = 0;   /* number of computed values (next free index) */
    static size_t T_cap = 0;   /* capacity of allocated array */

    /* Ensure the array is initialized for m = 0 */
    if (!T) {
        T_cap = 4;                 /* initial capacity */
        T_len = 1;                 /* we will store T[0] = 1 */
        T = (seq_t *)malloc(T_cap * sizeof(seq_t));
        if (!T) return (seq_t)0;   /* out of memory: return 0 as safe fallback */
        T[0] = (seq_t)1;
    }

    /* Compute the needed index m = 2*n + 1 */
    idx_t target = n;
    target = target * 2 + 1;

    /* Grow the array if necessary */
    if (T_len <= target) {
        /* Allocate a larger array */
        size_t new_cap = T_cap * 2;
        if (new_cap <= target + 1) new_cap = target + 1;
        seq_t *new_T = (seq_t *)realloc(T, new_cap * sizeof(seq_t));
        if (!new_T) return (seq_t)0;   /* out of memory: safe fallback */
        T = new_T;
        T_cap = new_cap;
        /* Compute new entries */
        for (idx_t m = T_len; m <= target; ++m) {
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;           /* C(m-1,0) */
            /* Sum over k = 0 .. m-1 of binom * T[k] */
            for (idx_t k = 0; k <= m - 1; ++k) {
                sum = sum + binom * T[k];
                if (k < m - 1) {
                    /* Update binom to C(m-1, k+1) */
                    seq_t num = (seq_t)(m - 1 - k);
                    seq_t den = (seq_t)(k + 1);
                    binom = binom * num;   /* multiply first to keep integer result */
                    binom = binom / den;
                }
            }
            T[m] = -sum;   /* T(m) = - Sum_{k=0}^{m-1} C(m-1,k) * T(k) */
        }
        T_len = target + 1;
    }

    return T[target];
}
