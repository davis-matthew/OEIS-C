/*
 * A383053  a(n) = Sum_{k=0..n} (k+1)^4 * Stirling2(n,k).
 * https://oeis.org/A383053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+1)^4 * Stirling2(n,k) (OEIS A383053)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the OEIS sequence A383053.
 * The function is memoised: values for n <= curr_max are cached,
 * and higher values are built incrementally. */
seq_t a(idx_t n)
{
    /* ---------- cache for a(n) sequence ---------- */
    static seq_t *a_seq = NULL;      /* a_seq[i] = a(i) */
    static size_t a_cap = 0;         /* capacity of a_seq */
    static idx_t  curr_max = 0;      /* largest n already computed */

    /* ---------- arrays for Stirling numbers ---------- */
    static seq_t *S_prev = NULL;       /* previous row, length prev_len */
    static seq_t *S_curr = NULL;       /* current row, reused each step */
    static size_t S_cap = 0;          /* capacity of both arrays          */
    static size_t prev_len = 0;        /* number of entries in S_prev      */

    /* initialise a_seq for n == 0 */
    if (a_seq == NULL) {
        a_cap = 1;
        a_seq = (seq_t*)malloc(a_cap * sizeof(seq_t));
        if (!a_seq) return 0;           /* out of memory */
        a_seq[0] = (seq_t)1;            /* a(0) = 1 */
    }

    /* initialise Stirling arrays for m == 0 */
    if (S_prev == NULL) {
        S_cap = 1;
        S_prev = (seq_t*)malloc(S_cap * sizeof(seq_t));
        if (!S_prev) return 0;
        S_curr = (seq_t*)malloc(S_cap * sizeof(seq_t));
        if (!S_curr) { free(S_prev); return 0; }
        S_prev[0] = (seq_t)1;   /* S(0,0) = 1 */
        prev_len = 1;
    }

    /* expand a_seq capacity if required */
    if (n >= a_cap) {
        size_t cap = a_cap;
        while (cap <= n) cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_seq, cap * sizeof(seq_t));
        if (!tmp) return 0;
        a_seq = tmp;
        a_cap = cap;
    }

    /* if we already have the value, return it */
    if (n <= curr_max) return a_seq[n];

    /* compute values up to n incrementally */
    for (idx_t m = curr_max + 1; m <= n; ++m) {
        /* ensure capacity for the new row */
        if (S_cap < (size_t)m + 1) {
            size_t cap = S_cap;
            while (cap <= (size_t)m) cap <<= 1;
            seq_t *tmp_prev = (seq_t*)realloc(S_prev, cap * sizeof(seq_t));
            seq_t *tmp_curr = (seq_t*)realloc(S_curr, cap * sizeof(seq_t));
            if (!tmp_prev || !tmp_curr) return 0;
            S_prev = tmp_prev;
            S_curr = tmp_curr;
            S_cap = cap;
        }

        /* S(m,0) = 0 for m > 0 */
        S_curr[0] = (seq_t)0;
        seq_t sum = (seq_t)0;

        for (idx_t k = 1; k <= m; ++k) {
            /* S(m,k) = k * S(m-1,k) + S(m-1,k-1) */
            seq_t part1 = (seq_t)k *
                          ( (k < (idx_t)prev_len) ? S_prev[(size_t)k] : (seq_t)0 );
            seq_t part2 = ( (k-1 < (idx_t)prev_len) ? S_prev[(size_t)(k-1)] : (seq_t)0 );
            S_curr[(size_t)k] = part1 + part2;

            /* add term (k+1)^4 * S_curr[k] to the sum */
            seq_t kp1   = (seq_t)k + (seq_t)1;
            seq_t kp14  = (kp1 * kp1) * (kp1 * kp1);
            sum += kp14 * S_curr[(size_t)k];
        }

        /* store the computed a(m) */
        a_seq[(size_t)m] = sum;
        curr_max = m;

        /* prepare for next iteration: swap rows */
        seq_t *tmp   = S_prev;
        S_prev       = S_curr;
        S_curr       = tmp;
        prev_len     = (size_t)m + 1;
    }

    return a_seq[(size_t)n];
}
