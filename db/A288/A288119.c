/*
 * A288119  Lexicographically earliest sequence of distinct nonnegative terms
 *          such that, for any i and j >= 0, a(i+j) != a(i) + a(j).
 * https://oeis.org/A288119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n % 9) + 9 * (n / 9) with base values a(0..8) =
 *            {1,0,2,3,5,4,8,7,6}
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool, not used here but required by guidelines */

seq_t a(idx_t n)
{
    /* Base values for indices 0..8 */
    static const seq_t base[9] = {
        (seq_t)1, (seq_t)0, (seq_t)2, (seq_t)3, (seq_t)5,
        (seq_t)4, (seq_t)8, (seq_t)7, (seq_t)6
    };

    idx_t r = n % 9;               /* remainder modulo 9 */
    seq_t block = (seq_t)(n / 9);   /* integer division, cast to seq_t */

    return base[r] + (block * (seq_t)9);
}
