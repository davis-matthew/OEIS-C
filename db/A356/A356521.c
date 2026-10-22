/*
 * A356521  The constant coefficient of (x + x*y + y + 1/(x*y))^n.
 * https://oeis.org/A356521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=ceil(n/3)..floor(n/2)}
 *            C(n-j,n-2j)*C(j,n-2j)*C(n,j)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* range for j: ceil(n/3) to floor(n/2) */
    idx_t j_start = (n + 2) / 3;   /* ceil(n/3) */
    idx_t j_end   = n / 2;         /* floor(n/2) */
    for (idx_t j = j_start; j <= j_end; ++j) {
        idx_t a = n - 2 * j;                      /* n-2j */
        seq_t term = binom(n - j, a);             /* C(n-j, n-2j) */
        term *= binom(j, a);                      /* * C(j, n-2j) */
        term *= binom(n, j);                      /* * C(n, j) */
        sum += term;
    }
    return sum;
}
