/*
 * A327564  If n = Product (p_j^k_j) then a(n) = Product ((p_j + 1)^(k_j -
 *          1)).
 * https://oeis.org/A327564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   For n = ∏ p_j^k_j, a(n) = ∏ (p_j + 1)^(k_j − 1)
 * Range:     u8 n <= 127, u16 n <= 4095, u32 n <= 9677, u64 n <= 9677,
 *            u128 n <= 9677, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1U)
        return (seq_t)1U;

    uint64_t t = (uint64_t)n;
    seq_t res = (seq_t)1U;

    for (uint64_t p = 2; p <= t / p; ) {
        uint32_t exp = 0;
        while (t % p == 0) {
            t /= p;
            exp++;
        }
        if (exp) {
            /* (p + 1)^(exp - 1) */
            res *= big_pow((seq_t)(p + 1), (seq_t)(exp - 1));
        }
        p = (p == 2) ? 3 : p + 2;   /* jump from 2 to 3, then odd numbers */
    }
    /* remaining prime factor (if any) has exponent 1 → contributes 1 */
    return res;
}
