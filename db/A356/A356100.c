/*
 * A356100  a(n) = Sum_{k=1..n} (k - 1)^n * floor(n/k).
 * https://oeis.org/A356100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (k - 1)^n * floor(n/k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t base = (seq_t)(k - 1);
        seq_t power = seq_pow(base, n);           /* (k-1)^n   */
        seq_t mult = (seq_t)(n / k);              /* floor(n/k) */
        result += power * mult;
    }
    return result;
}
