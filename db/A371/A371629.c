/*
 * A371629  If 2n = Sum 2^e(k) then a(n) = Sum e(k)^3.
 * https://oeis.org/A371629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Σ_{i : bit i of 2n = 1} i^3 (2n = Σ 2^e_k ⇒ a(n) = Σ
 *            e_k^3)
 * Range:     u8 n <= 39, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    idx_t bits  = n;       /* bits of n represent 2n shifted left by 1 */
    idx_t i     = 0;       /* current bit position in n */

    while (bits != 0) {
        if (bits & 1ULL) {
            seq_t pos  = (seq_t)(i + 1);   /* corresponding exponent in 2n */
            seq_t cube = pos * pos * pos;
            sum += cube;
        }
        bits >>= 1;
        ++i;
    }
    return sum;
}
