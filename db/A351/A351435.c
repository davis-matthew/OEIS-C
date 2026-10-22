/*
 * A351435  If n = Product (p_j^k_j) then a(n) = Product ((p_j + 1)^(k_j +
 *          1)).
 * https://oeis.org/A351435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^k∥n} (p+1)^(k+1)
 * Range:     u8 n <= 9, u16 n <= 137, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation performs a trial‑division factorisation of n
 * and multiplies the contributions of each prime factor.
 * It works for all builds where seq_t can be
 * multiplied with the normal `*` operator.               */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* Not part of the sequence, safety. */

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            seq_t factor = big_pow((seq_t)(p + 1), (seq_t)(cnt + 1));
            result *= factor;
        }
    }

    /* Remaining prime factor, if any. */
    if (temp > 1) {
        seq_t factor = big_pow((seq_t)(temp + 1), (seq_t)2);
        result *= factor;
    }

    return result;
}
