/*
 * A363703  Lower independence number of the n-folded cube graph.
 * https://oeis.org/A363703
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 for n = 2,3; a(n) = 4 for n = 4,5; For even n ≥ 6: a(n)
 *            = 3 * 2^(n-5); For odd n ≥ 7: a(n) = 2^(n-4)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 2 || n == 3) {
        return (seq_t)1;
    }
    if (n == 4 || n == 5) {
        return (seq_t)4;
    }
    if (n % 2 == 0) {                /* even n (n ≥ 6) */
        return (seq_t)3 * pow2(n - 5);
    } else {                         /* odd n (n ≥ 7) */
        return pow2(n - 4);
    }
}
