/*
 * A283122  a(n) = sigma(8*n).
 * https://oeis.org/A283122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(8*n)
 * Range:     u8 n <= 14, u16 n <= 2204, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = 8 * n  (the argument to sigma) */
    uint64_t m   = 8ULL * (uint64_t)n;
    uint64_t temp = m;
    seq_t result  = 1;

    /* Handle factor 2 separately */
    uint64_t exp2 = 0;
    while ((temp & 1ULL) == 0) {
        ++exp2;
        temp >>= 1;
    }
    if (exp2) {
        seq_t pow2e = 1;
        for (uint64_t i = 0; i < exp2; ++i) pow2e *= (seq_t)2;
        seq_t s = ((pow2e * (seq_t)2) - 1) / (seq_t)(2 - 1);
        result *= s;
    }

    /* Factor odd primes */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                ++exp;
                temp /= p;
            }
            seq_t ppow = 1;
            for (uint64_t i = 0; i < exp; ++i) ppow *= (seq_t)p;
            seq_t s = ((ppow * (seq_t)p) - 1) / (seq_t)(p - 1);
            result *= s;
        }
    }

    /* Remaining prime factor (if any) */
    if (temp > 1) {
        seq_t ppow = (seq_t)temp;
        seq_t s = ((ppow * (seq_t)temp) - 1) / (seq_t)(temp - 1);
        result *= s;
    }

    return result;
}
