/*
 * A348622  Triangular array read by rows: T(n,k) is the number of periodic n
 *          X n matrices over GF(2) having rank k, n>=0, 0<=k<=n.
 * https://oeis.org/A348622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 25, u64 n <= 49, u128 n <= 86,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A348622 - Periodic n×n matrices over GF(2)
 *
 *  Formula:
 *  a(n) = T(r,c)  where r = ⌊(√(8n+1)−1)/2⌋ and
 *          c = n − r(r+1)/2
 *  T(n,k) = ∏_{i=n−k+1}^{n} (2^{i−1}·(2^{i}−1))
 *
 *  This uses the recurrence T(n,k)=T(n,k−1)·T(n−k+1,1) and
 *  the closed form T(i,1)=2^{i−1}(2^{i}−1).
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

/* Integer square root using 128‑bit arithmetic (floor) */
static inline idx_t int_isqrt_u64(idx_t x) {
    idx_t r = 0;
    for (int b = 63; b >= 0; --b) {
        idx_t t = r + ((idx_t)1 << b);
        if ((unsigned __int128)t * t <= x) {
            r = t;
        }
    }
    return r;
}

/* Cached values of T(i,1) = 2^{i-1}(2^{i}-1) */
static seq_t *T1 = NULL;
static idx_t T1_len = 0;

static void ensure_T1(idx_t up_to) {
    if (T1_len <= up_to) {
        idx_t new_len = up_to + 1;
        seq_t *new_arr = (seq_t *)realloc(T1, new_len * sizeof(seq_t));
        if (!new_arr) { /* allocation failed – terminate */
            exit(EXIT_FAILURE);
        }
        T1 = new_arr;
        for (idx_t i = T1_len; i <= up_to; ++i) {
            if (i == 0) {
                T1[i] = 1;  /* unused but harmless */
            } else {
                seq_t p_i   = pow2(i);         /* 2^i   */
                seq_t p_li  = pow2(i - 1);     /* 2^{i-1} */
                T1[i] = p_li * (p_i - 1);     /* 2^{i-1} * (2^i - 1) */
            }
        }
        T1_len = new_len;
    }
}

/* Main function: compute a(n) */
seq_t a(idx_t n) {
    /* Determine row r and column c */
    idx_t r = (int_isqrt_u64(8 * n + 1) - 1) / 2;
    idx_t S_prev = r * (r + 1) / 2;
    idx_t c = n - S_prev;           /* 0 <= c <= r */

    /* Ensure T1 values up to r are computed */
    ensure_T1(r);

    /* Compute product ∏_{i=r−c+1}^{r} T1[i] */
    seq_t prod = 1;
    if (c > 0) {
        idx_t start = r - c + 1;
        for (idx_t i = start; i <= r; ++i) {
            prod *= T1[i];
        }
    }
    return prod;
}
