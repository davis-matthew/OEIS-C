/*
 * A398658  Upper (1, 3/2) midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 7*a(n-4) + 4*a(n-5) -
 *            a(n-6), a(0) = 0, a(1) = 3, a(2) = 11, a(3) = 27, a(4) = 55,
 *            a(5) = 98
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial values for n = 0 .. 5 */
    static const seq_t base[6] = { (seq_t)0, (seq_t)3, (seq_t)11,
                                   (seq_t)27, (seq_t)55, (seq_t)98 };

    if (n < 6)
        return base[n];

    /* buffer holds the last six terms: a(k-6) … a(k-1) */
    seq_t cur[6];
    for (idx_t i = 0; i < 6; ++i)
        cur[i] = base[i];

    for (idx_t k = 6; k <= n; ++k)
    {
        /* compute a(k) = 4*a(k-1) - 7*a(k-2) + 8*a(k-3) - 7*a(k-4) + 4*a(k-5) - a(k-6) */
        seq_t pos = 4 * cur[5] + 8 * cur[3] + 4 * cur[1];
        seq_t neg = 7 * cur[4] + 7 * cur[2] + cur[0];
        seq_t next = pos - neg;

        /* shift window: drop a(k-6), move others left, insert next */
        cur[0] = cur[1];
        cur[1] = cur[2];
        cur[2] = cur[3];
        cur[3] = cur[4];
        cur[4] = cur[5];
        cur[5] = next;
    }

    return cur[5];
}
