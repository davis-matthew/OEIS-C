/*
 * A321125  T(n,k) = b(n+k) - (2*b(n)*b(k) + 1)*b(n*k) + b(n) + b(k) + 1,
 *          where b(n) = A154272(n+1), square array read by antidiagonals (n
 *          >= 0, k >= 0).
 * https://oeis.org/A321125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11475, u16 n <= 11475, u32 n <= 11475, u64 n <= 11475,
 *            u128 n <= 11475, big any n
 * Validated: standard, pass, 107 terms through n = 9924 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A321125
 *  a(n) is obtained by reading the array T(n,k) along its anti‑diagonals.
 *  T(n,k) satisfies the closed formula
 *
 *      T(n,k) = 1                                 if n==0 || k==0
 *      T(n,k) = 1 + (n==2) + (k==2)               if n,k>0 and n*k!=2
 *      T(n,k) = 1 + (n==2) + (k==2) - 1           if n*k==2
 *
 *  Because the sequence A154272(n+1) (the function b) is only 1 at n=0 and at n=2
 *  and 0 otherwise, the above reduces to the simple cases above.  The function
 *  below uses the provided helper `antidiagonal_coords` to obtain the
 *  coordinates (i,j) corresponding to the index n in the anti‑diagonal
 *  traversal.
 */

#include <stdbool.h>

/* mapping from linear index n to pair (i,j) on the antidiagonals of T */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* find s such that (s+1)(s+2)/2 <= n < (s+2)(s+3)/2 */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n) ++s;
    idx_t pos = n - (s * (s + 1)) / 2;  /* 0 <= pos <= s */
    *i = pos;
    *j = s - pos;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    /* Special case: edge of the array. */
    if (i == 0 || j == 0)
        return 1;

    /* Small auxiliary values for the 0/1 indicator of i or j being 2. */
    seq_t bn = (i == 2) ? 1 : 0;   /* b(i) */
    seq_t bk = (j == 2) ? 1 : 0;   /* b(j) */

    /* Compute the three terms of the simplified formula. */
    seq_t term1 = (i + j == 2) ? 1 : 0;                   /* b(i+j) */
    idx_t prod = i * j;
    seq_t bnk  = (prod == 2) ? 1 : 0;                     /* b(i*j) */
    seq_t term2 = ( (2 * bn * bk + 1) * bnk );            /* (2*b(i)*b(j)+1)*b(i*j) */

    /* Compute the final value.  The arithmetic is tiny, so we safely use a
     * signed intermediate to avoid underflow on unsigned types. */
    int64_t temp = (int64_t)term1 - (int64_t)term2
                 + (int64_t)bn + (int64_t)bk + 1;

    return (seq_t)temp;
}
