/*
 * A355098  E.g.f. A(x) satisfies A(x) = 1 - 2 * log(1-x) * A(-log(1-x)).
 * https://oeis.org/A355098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{k=1..n} k * |Stirling1(n,k)| * a(k-1), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses unsigned Stirling numbers of the first kind computed iteratively. */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n) {
    /* ---------- static storage for memoization ---------- */
    static seq_t *a_arr = NULL;   /* computed a values */
    static idx_t a_len  = 0;      /* number of stored a values (>=1) */

    static seq_t *s_prev = NULL;  /* Stirling numbers for n-1 */
    static seq_t *s_cur  = NULL;  /* Stirling numbers for current n */
    static idx_t  s_len  = 0;     /* allocated length of Stirling arrays */

    /* ---------- allocation / resizing ---------- */
    if (a_len == 0) {        /* first call: initialize for n=0 */
        a_len = 1;
        a_arr = (seq_t*)malloc(sizeof(seq_t) * 1);
        a_arr[0] = 1;         /* a(0) = 1 */
        /* allocate Stirling arrays of size 1 */
        s_len = 1;
        s_prev = (seq_t*)malloc(sizeof(seq_t) * 1);
        s_cur  = (seq_t*)malloc(sizeof(seq_t) * 1);
        s_prev[0] = 1;        /* s[0][0] = 1 */
    }

    /* Ensure we have capacity up to n */
    if ((idx_t)n >= a_len) {
        idx_t new_len = (idx_t)n + 1;
        /* resize a_arr */
        a_arr = (seq_t*)realloc(a_arr, sizeof(seq_t) * new_len);
        /* resize Stirling arrays */
        if (new_len > s_len) {
            s_prev = (seq_t*)realloc(s_prev, sizeof(seq_t) * new_len);
            s_cur  = (seq_t*)realloc(s_cur,  sizeof(seq_t) * new_len);
            /* zero new entries */
            for (idx_t i = s_len; i < new_len; ++i) {
                s_prev[i] = 0;
                s_cur[i]  = 0;
            }
            s_len = new_len;
        }
        /* Compute missing a values */
        for (idx_t i = a_len; i <= (idx_t)n; ++i) {
            /* Compute Stirling numbers for row i */
            s_cur[0] = 0;                     /* |s(i,0)| = 0 for i>0 */
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term1 = (seq_t)(i - 1) * s_prev[k]; /* (i-1)*s_{i-1,k} */
                seq_t term2 = s_prev[k - 1];              /* s_{i-1,k-1} */
                s_cur[k] = term1 + term2;                 /* |s(i,k)| */
            }
            /* Compute a(i) from recurrence */
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                /* ((k) * s_cur[k]) * a(k-1) */
                seq_t prod = (seq_t)k * s_cur[k];
                prod *= a_arr[k - 1];
                sum += prod;
            }
            a_arr[i] = sum * 2;   /* a(i) = 2 * sum */
            /* Swap s_prev and s_cur for next iteration */
            seq_t *tmp = s_prev;
            s_prev = s_cur;
            s_cur = tmp;
        }
        a_len = n + 1;
    }

    return a_arr[n];
}
