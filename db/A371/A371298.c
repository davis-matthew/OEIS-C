/*
 * A371298  E.g.f. satisfies A(x) = 2/(3 - exp(2*x*A(x)^2)).
 * https://oeis.org/A371298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2*n+1)!) * Sum_{k=0..n} 2^(n-k) * (2*n+k)! *
 *            Stirling2(n,k) (the Stirling numbers of the second kind are
 *            computed on the fly). The implementation uses O(n^2) arithmetic
 *            operations and memoises all values up to the largest n requested
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of previously computed values */
    static seq_t *S_prev = NULL;   /* previous Stirling row */
    static seq_t *S_curr = NULL;   /* current Stirling row */
    static seq_t *A_vals = NULL;   /* a(n) values */
    static seq_t *pow2_arr = NULL; /* powers of 2 */
    static idx_t cap = 0;          /* current allocation size */
    static idx_t last_n = (idx_t)-1; /* largest n already computed */

    /* initial allocation */
    if (cap == 0) {
        cap = 1; /* at least for n = 0 */
        S_prev   = (seq_t*)malloc(cap * sizeof(seq_t));
        S_curr   = (seq_t*)malloc(cap * sizeof(seq_t));
        A_vals   = (seq_t*)malloc(cap * sizeof(seq_t));
        pow2_arr = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!S_prev || !S_curr || !A_vals || !pow2_arr) abort();
        S_prev[0]   = (seq_t)1;   /* S(0,0) = 1 */
        S_curr[0]   = (seq_t)0;   /* will never be used for n=0 */
        A_vals[0]   = (seq_t)1;   /* a(0) = 1 */
        pow2_arr[0] = (seq_t)1;   /* 2^0 = 1 */
        last_n = 0;
    }

    /* enlarge cache if necessary */
    if (n + 1 > cap) {
        /* grow to the next power of two for simplicity */
        idx_t newcap = cap;
        while (newcap <= n) newcap <<= 1;
        S_prev = (seq_t*)realloc(S_prev, newcap * sizeof(seq_t));
        S_curr = (seq_t*)realloc(S_curr, newcap * sizeof(seq_t));
        A_vals = (seq_t*)realloc(A_vals, newcap * sizeof(seq_t));
        pow2_arr = (seq_t*)realloc(pow2_arr, newcap * sizeof(seq_t));
        if (!S_prev || !S_curr || !A_vals || !pow2_arr) abort();
        cap = newcap;
    }

    /* extend pow2 array to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (i >= cap) abort(); /* safety check */
        pow2_arr[i] = pow2_arr[i-1] * (seq_t)2;
    }

    /* compute values up to n */
    while (last_n < n) {
        idx_t m = last_n + 1;          /* new n to compute */
        idx_t prev_n = last_n;         /* size of previous Stirling row */

        /* compute Stirling numbers S(m, k), k = 0..m */
        for (idx_t k = 0; k <= m; ++k) {
            if (k == 0) {
                S_curr[0] = (seq_t)0; /* S(m,0) = 0 for m>0 */
            } else {
                seq_t term1 = (k <= prev_n) ? S_prev[k] * (seq_t)k : (seq_t)0;
                seq_t term2 = (k-1 <= prev_n) ? S_prev[k-1] : (seq_t)0;
                S_curr[k] = term1 + term2;
            }
        }

        /* compute a(m) = Sum_{k=1..m} 2^(m-k) * Q_k * S_curr[k] */
        seq_t sum = (seq_t)0;
        seq_t Q   = (seq_t)1; /* Q_k = product_{i=2..k} (2m + i) */

        for (idx_t k = 1; k <= m; ++k) {
            seq_t pow = pow2_arr[m - k]; /* 2^(m-k) */
            sum = sum + (S_curr[k] * Q * pow);
            if (k < m) {
                Q = Q * (seq_t)(2 * m + k + 1); /* multiply by (2m + (k+1)) */
            }
        }
        A_vals[m] = sum;

        /* prepare for next iteration: swap rows */
        seq_t *tmp = S_prev;
        S_prev = S_curr;
        S_curr = tmp;

        last_n = m;
    }

    return A_vals[n];
}
