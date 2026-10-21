/*
 * A244750  0-additive sequence: a(n) is the smallest number larger than
 *          a(n-1) which is not the sum of any subset of earlier terms, with
 *          initial values {0, 2, 3, 4}.
 * https://oeis.org/A244750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=2, a(3)=3, a(4)=4; for n>4 a(n)=2*a(n-1)=2^(n-2)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 4) {
        switch ((unsigned)n) {
            case 1: return (seq_t)0;
            case 2: return (seq_t)2;
            case 3: return (seq_t)3;
            case 4: return (seq_t)4;
            default: return (seq_t)0; /* unreachable */
        }
    }
    seq_t two = (seq_t)2;
    seq_t exp = (seq_t)(n - 2);
    return big_pow(two, exp);
}
