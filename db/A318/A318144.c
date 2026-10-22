/*
 * A318144  T(n, k) = (-1)^k*k!*P(n, k), where P(n, k) is the number of
 *          partitions of n of length k. Triangle read by rows, 0 <= k <= n.
 * https://oeis.org/A318144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n‑th element of the triangle T(n,k)=(-1)^k * k! *
 *            P(n,k) (P = number of partitions of n of length k), read in
 *            row–major order. For each n there is a unique pair (r,k) with
 *            r(r+1)/2 ≤ n < (r+1)(r+2)/2; then a(n)=(-1)^k * k! * P(r,k). The
 *            implementation precomputes all P(r,k) up to the...
 * Range:     i8 n <= 26, i16 n <= 43, i32 n <= 103, i64 n <= 251,
 *            i128 n <= 628, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* --------------------------------------------------------------------- */
/*  storage for the triangular array of partition numbers  P(n,k)        */
/* --------------------------------------------------------------------- */

static seq_t *P_data   = NULL;      /* 1‑D array: P(row, k) */
static size_t P_alloc  = 0;          /* allocated size of P_data          */
static idx_t   P_max    = 0;          /* largest n that has been computed  */

/* offset of the beginning of row n in the 1‑D array (n*(n+1)/2) */
static inline size_t offset_of(idx_t n) {
    return (size_t)n * (n + 1) / 2;
}

/* allocate / reallocate the partition array up to max_n (inclusive)    */
static void compute_partitions(idx_t max_n) {
    size_t needed = offset_of(max_n) + (size_t)max_n + 1;
    if (needed > P_alloc) {
        seq_t *new_data = (seq_t *)realloc(P_data, needed * sizeof(seq_t));
        if (!new_data) abort();
        P_data = new_data;
        P_alloc = needed;
    }
    for (idx_t n = 0; n <= max_n; ++n) {
        size_t row_off = offset_of(n);
        for (idx_t k = 0; k <= n; ++k) {
            size_t idx = row_off + k;
            if (n == 0 && k == 0) {
                P_data[idx] = (seq_t)1;
            } else if (k == 0) {
                P_data[idx] = (seq_t)0;
            } else {
                seq_t val = (seq_t)0;
                /* P[n-1][k-1] */
                val += P_data[offset_of(n - 1) + (k - 1)];
                /* P[n-k][k]  (only if n-k >= k) */
                if (n - k >= k)
                    val += P_data[offset_of(n - k) + k];
                P_data[idx] = val;
            }
        }
    }
    P_max = max_n;
}

/* factorial for 0 ≤ k ≤ max needed (≈ sqrt(2*n)) */
static seq_t factorial_of(idx_t k) {
    seq_t res = (seq_t)1;
    for (idx_t i = 2; i <= k; ++i)
        res = res * (seq_t)i;
    return res;
}

/* --------------------------------------------------------------------- */
/*  requested function: value of the sequence at index n                 */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n) {
    /* initialize partition table for row 0 if not yet done */
    if (P_max == 0 && (n == 0 || n < (idx_t)offset_of(1)))
        compute_partitions(0);

    /* find the row r where the n‑th element lies   */
    double d = sqrt(8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* correct possible rounding errors around the boundary */
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) ++r;
    while ((unsigned __int128)r * (r + 1) / 2 > n) --r;

    idx_t k = n - (unsigned __int128)r * (r + 1) / 2U;

    /* ensure we have all partitions up to row r */
    if (r > P_max)
        compute_partitions(r);

    seq_t part = P_data[offset_of(r) + k];
    seq_t fac  = factorial_of(k);
    seq_t val  = fac * part;
    if (k & 1)
        val = -val;
    return val;
}
