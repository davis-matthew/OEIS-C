/*
 * A260851  a(n) in base n is the concatenation of the base n expansions of
 *          (1, 2, 3, ..., n-1, n, n-1, ..., 3, 2, 1).
 * https://oeis.org/A260851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (( (n^n - 1) / (n - 1) ) - n + 1) * (1 + n * ((n^n - 1) /
 *            (n - 1))) - 1, for n ≥ 1; a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 1;   /* a(1) = 1 (a(0) not defined, return 1 for safety) */
    }

    seq_t sn = (seq_t)n;
    seq_t pow_n_n = big_pow(sn, sn);          /* n^n */
    seq_t r = (pow_n_n - 1) / (n - 1);        /* repunit (n^n - 1)/(n-1) */
    seq_t one_plus_n_r = 1 + sn * r;          /* 1 + n*r */
    seq_t factor = r - sn + 1;                /* r - n + 1 */
    return factor * one_plus_n_r - 1;
}
