/*
 * A286099  Square array read by antidiagonals: A(n,k) = T(n OR k, n AND k),
 *          where T(n,k) is sequence A001477 considered as a two-dimensional
 *          table, AND is bitwise-and (A004198) and OR is bitwise-or
 *          (A003986).
 * https://oeis.org/A286099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = Cantor(i & j, i | j), * where (i, j) are the
 *            antidiagonal coordinates of index n * given by
 *            antidiagonal_coords. The Cantor pairing function is * T(x, y) =
 *            (x + y)·(x + y + 1)/2 + y. Thus * * a(n) = ((i & j + i | j) * (i
 *            & j + i | j + 1)) / 2 + (i | j)
 * Range:     u8 n <= 252, u16 n <= 10584, u32 n <= 10584, u64 n <= 10584,
 *            u128 n <= 10584, big any n
 * Validated: standard, pass, 107 terms through n = 9519 (2026-11-01)
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

    seq_t and_val = (seq_t)(i & j);
    seq_t or_val  = (seq_t)(i | j);

    seq_t sum = and_val + or_val;
    seq_t prod = sum * (sum + (seq_t)1) / (seq_t)2;
    seq_t result = prod + or_val;

    return result;
}
