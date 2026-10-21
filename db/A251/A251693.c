/*
 * A251693  a(n) = (n+1) * (2*n+1)^(n-2) * 3^n.
 * https://oeis.org/A251693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * (2*n+1)^(n-2) * 3^n (n ≥ 2) Base cases: a(0) = 1,
 *            a(1) = 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;

    seq_t sn = (seq_t)n;                 // convert index to seq_t
    seq_t base1 = (seq_t)(2*n + 1);     // (2*n+1)
    seq_t exp1  = sn - 2;               // n-2
    seq_t pow1  = big_pow(base1, exp1); // (2*n+1)^(n-2)

    seq_t pow3 = big_pow((seq_t)3, sn); // 3^n

    seq_t result = (sn + 1) * pow1;     // (n+1) * (2*n+1)^(n-2)
    result = result * pow3;             // * 3^n

    return result;
}
