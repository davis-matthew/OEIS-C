/*
 * A290187  Minimum number of moves to rearrange a Tower of Hanoi puzzle with
 *          3 colors. There are 3 rods with n discs in descending sizes. On
 *          the first rod there are n red discs, on the 2nd n blue discs and
 *          on the 3rd n white ones.
 * https://oeis.org/A290187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 432*2^(n-5) - 8*n - 15 for n ≥ 5; a(1) = 5, a(2) = 23,
 *            a(3) = 68, a(4) = 168
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset‑1 handling, not used in tests */
        return (seq_t)0;

    if (n <= 4) {                   /* small precomputed values */
        static const seq_t base[4] = {
            (seq_t)5, (seq_t)23, (seq_t)68, (seq_t)168
        };
        return base[n - 1];
    }

    /* compute 432*2^(n-5) - 8*n - 15  for n ≥ 5 */
    int exp = (int)(n - 5ULL);      /* exponent fits in a signed 32‑bit int for n ≤ 10000 */
    seq_t pow2 = pow_seq((seq_t)2, exp);
    seq_t val   = pow2 * (seq_t)432;
    seq_t sub   = (seq_t)8 * (seq_t)n + (seq_t)15;
    return val - sub;
}
