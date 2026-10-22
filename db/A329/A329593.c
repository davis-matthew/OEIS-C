/*
 * A329593  a(n) = (2^(A003558(n)) - A332433(n))/(2*n+1), for n >= 0.
 * https://oeis.org/A329593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(A003558(n)) - A332433(n))/(2*n+1)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 32, u64 n <= 73, u128 n <= 145,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Special case n = 0: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* m = 2*n + 1  (fits into idx_t) */
    idx_t m = 2 * n + 1;

    /* Find the smallest k with 2^k ≡ ±1 (mod m) */
    idx_t k = 1;
    idx_t modpow = 2 % m;          /* 2^1 mod m, m > 1 here */

    while (modpow != 1 && modpow != m - 1) {
        modpow = (modpow * 2) % m;
        ++k;
    }

    /* sign = +1 if 2^k ≡ 1, -1 if 2^k ≡ -1 (i.e. m-1) */
    int sign = (modpow == 1) ? 1 : -1;

    /* Compute 2^k exactly (big_pow handles all seq_t sizes) */
    seq_t powk = big_pow((seq_t)2, (seq_t)k);

    /* Numerator is 2^k - sign, where sign is ±1 */
    seq_t numerator = (sign == 1) ? (powk - (seq_t)1) : (powk + (seq_t)1);

    /* Divide by m to get a(n) */
    seq_t result = numerator / (seq_t)m;
    return result;
}
