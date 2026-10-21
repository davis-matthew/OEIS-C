/*
 * A290265  The number of non-palindromic Motzkin paths of length n.
 * https://oeis.org/A290265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = M(n) - Q(floor(n/2)) where M(n) is the nth Motzkin number
 *            and Q(k) is the number of Motzkin prefixes of length k that
 *            never go below the x‑axis
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes for C compatibility */
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* External linkage */
seq_t a(idx_t n)
{
    /*----- Pre-compute Motzkin numbers M(n) -----*/
#ifdef __cplusplus
    /* C++: use std::vector */
    static std::vector<seq_t> M = { (seq_t)1, (seq_t)1 };   /* M[0]=1, M[1]=1 */
    if (M.size() <= n) {
        M.resize(n + 1);
        for (idx_t i = M.size() - 1; i <= n; ++i) {
            seq_t a = (seq_t)((2 * i + 1));
            seq_t b = (seq_t)((3 * i - 3));
            seq_t num = a * M[i - 1] + b * M[i - 2];
            seq_t den = (seq_t)(i + 2);
            M[i] = num / den;
        }
    }
#else
    /* C: allocate dynamically */
    static seq_t *M = NULL;
    static size_t M_size = 0;
    if (M_size == 0) {
        M = (seq_t *)malloc(2 * sizeof(seq_t));
        M[0] = (seq_t)1;
        M[1] = (seq_t)1;
        M_size = 2;
    }
    if (M_size <= n) {
        M = (seq_t *)realloc(M, (n + 1) * sizeof(seq_t));
        for (idx_t i = M_size; i <= n; ++i) {
            seq_t a = (seq_t)((2 * i + 1));
            seq_t b = (seq_t)((3 * i - 3));
            seq_t num = a * M[i - 1] + b * M[i - 2];
            seq_t den = (seq_t)(i + 2);
            M[i] = num / den;
        }
        M_size = n + 1;
    }
#endif

    /*----- Pre-compute prefix counts Q(k) -----*/
#ifdef __cplusplus
    /* C++: use std::vector */
    static std::vector<seq_t> Q = { (seq_t)1 };           /* Q[0] = 1 */
    static std::vector<seq_t> dp_cur = { (seq_t)1 };      /* dp_cur for k=0 */
    idx_t k = n >> 1;  /* floor(n/2) */
    while (Q.size() <= k) {
        int t = (int)Q.size();   /* new k value */
        std::vector<seq_t> dp_next(t + 1);
        for (int j = 0; j <= t; ++j) {
            seq_t val = 0;
            if (j <= t - 1) val += dp_cur[j];     /* flat from same height */
            if (j > 0)     val += dp_cur[j - 1];   /* up from j-1 */
            if (j + 1 <= t - 1) val += dp_cur[j + 1]; /* down from j+1 */
            dp_next[j] = val;
        }
        seq_t sum = 0;
        for (int j = 0; j <= t; ++j) sum += dp_next[j];
        Q.push_back(sum);
        dp_cur.swap(dp_next);
    }
#else
    /* C: allocate dynamically */
    static seq_t *Q = NULL;
    static size_t Q_size = 0;
    static seq_t *dp_cur = NULL;
    static size_t dp_cur_size = 0;
    idx_t k = n >> 1;
    if (Q_size == 0) {
        /* initialize for k = 0 */
        Q = (seq_t *)malloc(sizeof(seq_t));
        Q[0] = (seq_t)1;
        Q_size = 1;
        dp_cur = (seq_t *)malloc(sizeof(seq_t));
        dp_cur[0] = (seq_t)1;
        dp_cur_size = 1;
    }
    while (Q_size <= k) {
        size_t t = Q_size;                /* new k value */
        seq_t *dp_next = (seq_t *)malloc((t + 1) * sizeof(seq_t));
        for (size_t j = 0; j <= t; ++j) {
            seq_t val = 0;
            if (j <= t - 1) val += dp_cur[j];                 /* flat */
            if (j > 0)     val += dp_cur[j - 1];              /* up */
            if (j + 1 <= t - 1) val += dp_cur[j + 1];        /* down */
            dp_next[j] = val;
        }
        /* compute sum for Q[t] */
        seq_t sum = 0;
        for (size_t j = 0; j <= t; ++j) sum += dp_next[j];
        /* grow Q array */
        Q = (seq_t *)realloc(Q, (Q_size + 1) * sizeof(seq_t));
        Q[Q_size] = sum;
        ++Q_size;
        /* replace dp_cur */
        free(dp_cur);
        dp_cur = dp_next;
        dp_cur_size = t + 1;
    }
#endif

    /*----- Compute final result -----*/
    seq_t result;
#ifdef __cplusplus
    result = M[n] - Q[k];
#else
    result = M[n] - Q[k];
#endif
    return result;
}
