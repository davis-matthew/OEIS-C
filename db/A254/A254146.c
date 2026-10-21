/*
 * A254146  a(n) = 1*8^n + 2*7^n + 3*6^n + 4*5^n + 5*4^n + 6*3^n + 7*2^n +
 *          8*1^n.
 * https://oeis.org/A254146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1*8^n + 2*7^n + 3*6^n + 4*5^n + 5*4^n + 6*3^n + 7*2^n +
 *            8*1^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 1; k <= 8; ++k) {
        seq_t base  = (seq_t)(9 - k);        /* 8,7,…,1 */
        seq_t power = seq_pow(base, n);      /*  base^n */
        result += (seq_t)k * power;          /* i * base^n */
    }
    return result;
}
