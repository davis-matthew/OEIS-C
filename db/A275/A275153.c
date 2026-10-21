/*
 * A275153  A linear-recurrent solution to Hofstadter's Q-recurrence.
 * https://oeis.org/A275153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3) = 0, a(4) = 0; otherwise let q = (n-1)/9 and r = (n-1)%9 (0
 *            ≤ r < 9) a(9q+1) = 9 a(9q+2) = 9q a(9q+3) = 3 a(9q+4) = 9
 *            a(9q+5) = (9q^2 + 9q + 14)/2 a(9q+6) = 9 a(9q+7) = 9 a(9q+8) =
 *            10 * 2^q a(9q+9) = 4
 * Range:     u8 n <= 49, u16 n <= 124, u32 n <= 268, u64 n <= 556,
 *            u128 n <= 1000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 3 || n == 4)
        return 0;

    idx_t m = n - 1;
    idx_t q = m / 9;
    idx_t r = m % 9;

    switch (r) {
    case 0:   /* n = 9q+1 */
        return 9;
    case 1:   /* n = 9q+2 */
        return (seq_t)9 * q;
    case 2:   /* n = 9q+3 */
        return 3;
    case 3:   /* n = 9q+4 */
        return 9;
    case 4:   /* n = 9q+5 */
    {
        seq_t num = (seq_t)9 * q * q + (seq_t)9 * q + 14;
        return num / 2;
    }
    case 5:   /* n = 9q+6 */
        return 9;
    case 6:   /* n = 9q+7 */
        return 9;
    case 7:   /* n = 9q+8 */
        return (seq_t)10 * pow2(q);
    case 8:   /* n = 9q+9 */
        return 4;
    default:  /* unreachable */
        return 0;
    }
}
