/*
 * A362302  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = n! * Sum_{j=0..floor(n/3)} (-k/6)^j *
 *          binomial(n-2*j,j)/(n-2*j)!.
 * https://oeis.org/A362302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is obtained from the square array T(n,k) where a(n) =
 *            T(i,k) with (i,k) determined by the n-th term in an antidiagonal
 *            traversal: for each integer t ≥ 0 the terms of diagonal t (i+j =
 *            t) appear in order i = 0,1,…,t. Recurrence for T:
 *            T(0,k)=T(1,k)=T(2,k)=1, T(m,k)=T(m-1,k) - k * C(m-1,2) * T...
 * Range:     i8 n <= 51, i16 n <= 100, i32 n <= 205, i64 n <= 460,
 *            i128 n <= 1121, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 1. Locate the (n)-th entry in the antidiagonal traversal. */
    idx_t t = 0;                           /* diagonal number */
    while ( ((__int128)(t + 1) * (t + 2)) / 2 <= n )
        ++t;
    idx_t prev = (t * (t + 1)) / 2;         /* number of terms before diagonal t */
    idx_t pos  = n - prev;                  /* 0‑based position in diagonal t */
    idx_t i = pos;                         /* row index */
    idx_t k = t - pos;                     /* column index, cast to seq_t later */

    /* 2. Compute T(i,k) by the recurrence. */
    if (i == 0) return 1;
    if (i == 1) return 1;
    if (i == 2) return 1;

    seq_t T[3];
    T[0] = 1;   /* T(0,k) */
    T[1] = 1;   /* T(1,k) */
    T[2] = 1;   /* T(2,k) */

    seq_t kval = (seq_t)k;
    for (idx_t m = 3; m <= i; ++m) {
        seq_t prev1 = T[(m - 1) % 3];
        seq_t prev3 = T[(m - 3) % 3];
        seq_t bin   = ((seq_t)(m - 1) * (m - 2)) / 2;  /* C(m-1,2) */
        seq_t sub   = kval * bin * prev3;
        T[m % 3] = prev1 - sub;
    }
    return T[i % 3];
}
