/*
 * A333905  Lexicographically earliest sequence of distinct positive integers
 *          such that a(n) divides the concatenation of a(n+1) to a(n+2).
 * https://oeis.org/A333905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1,a(2)=2,a(3)=3,a(4)=4,a(5)=5,a(6)=6; For n ≥ 7, a(n) =
 *            2·a(n-2)
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static const seq_t first[6] = { (seq_t)1, (seq_t)2, (seq_t)3,
                                    (seq_t)4, (seq_t)5, (seq_t)6 };
    if (n <= 6) {
        return first[n - 1];
    }
    if (n & 1U) {          /* odd n ≥ 7 */
        idx_t exp = (n - 7) / 2;
        return pow2(exp) * (seq_t)10;
    } else {               /* even n ≥ 8 */
        idx_t exp = (n - 8) / 2;
        return pow2(exp) * (seq_t)8;
    }
}
