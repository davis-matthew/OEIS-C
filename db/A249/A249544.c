/*
 * A249544  Array read by antidiagonals: T(m,n) read in binary is a palindrome
 *          with m runs of n ones separated by single zeros.
 * https://oeis.org/A249544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) read by antidiagonals of array T(m,n) where T(m,n) =
 *            ((2^(n+1))^m - 1)*(2^n - 1)/(2^(n+1) - 1)
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 49, u64 n <= 110, u128 n <= 219,
 *            big any n
 * Helpers:   big_pow, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/big_pow.h"

seq_t a(idx_t idx)
{
    /* find the diagonal s such that (s‑1)*s/2 >= idx   */
    idx_t s = 1;
    while ((s - 1) * s / 2 < idx) ++s;

    /* elements before this diagonal */
    idx_t prev   = (s - 2) * (s - 1) / 2;
    idx_t offset = idx - prev - 1;      /* 0‑based offset on this diagonal */
    idx_t m      = offset + 1;
    idx_t n      = s - m;               /* n ≥ 1 */

    /* compute powers of two */
    seq_t two_n_plus1 = pow_uint(2ULL, n + 1);
    seq_t two_n       = pow_uint(2ULL, n);

    /* A = 2^(n+1) */
    seq_t numerator   = big_pow(two_n_plus1, (seq_t)m);
    numerator -= 1;
    seq_t denominator = two_n_plus1 - 1;

    seq_t coeff   = numerator / denominator;   /* exact division */
    seq_t result  = coeff * (two_n - 1);
    return result;
}
