/*
 * A321126  T(n,k) = max(n + k - 1, n + 1, k + 1), square array read by
 *          antidiagonals (n >= 0, k >= 0).
 * https://oeis.org/A321126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i,j) where (i,j) are the antidiagonal coordinates of n
 *            * (i.e., the pair (n,k) visited while reading the square array *
 *            T(n,k)=max(n+k-1,n+1,k+1) row‑by‑row by antidiagonals)
 * Range:     u8 n <= 11475, u16 n <= 11475, u32 n <= 11475, u64 n <= 11475,
 *            u128 n <= 11475, big any n
 * Validated: standard, pass, 107 terms through n = 9303 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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

    /* Compute the three candidates for T(i,j) */
    seq_t term1;
    if (i + j == 0)
        term1 = (seq_t)0;          /* handle n=j=0 case */
    else
        term1 = (seq_t)(i + j - 1);

    seq_t term2 = (seq_t)(i + 1);
    seq_t term3 = (seq_t)(j + 1);

    /* Return the maximum of the three terms */
    seq_t res = term1;
    if (term2 > res) res = term2;
    if (term3 > res) res = term3;
    return res;
}
