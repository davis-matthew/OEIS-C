/*
 * A382513  Expansion of Sum_{p prime} p * x^p / (1 - p * x^p).
 * https://oeis.org/A382513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p^(n/p)
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    idx_t original = n;
    idx_t temp = n;

    /* Handle the prime factor 2 */
    if ((temp & 1) == 0) {
        seq_t exp = (seq_t)(original / 2);
        result += big_pow((seq_t)2, exp);
        while ((temp & 1) == 0) temp >>= 1;
    }

    /* Check odd potential factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            seq_t exp = (seq_t)(original / p);
            result += big_pow((seq_t)p, exp);
            while (temp % p == 0) temp /= p;
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        seq_t exp = (seq_t)(original / temp);
        result += big_pow((seq_t)temp, exp);
    }

    return result;
}
