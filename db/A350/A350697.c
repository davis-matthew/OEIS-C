/*
 * A350697  Smallest number m > 1 such that n * m = A350538(n) contains only
 *          even digits.
 * https://oeis.org/A350697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest integer m > 1 such that n * m consists only
 *            of even decimal digits
 * Range:     u8 n <= 8, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not part of the sequence, but protects against zero input */

    for (idx_t m = 2;; ++m) {
        /* compute product n*m in the proper type */
        seq_t prod = (seq_t)n * (seq_t)m;

        /* check that every decimal digit of prod is even */
        seq_t temp = prod;
        bool all_even = true;
        /* prod is always > 0 because n>0 and m>0 */
        while (temp != 0) {
            seq_t d = temp % 10;
            if ((d & 1) != 0) {   /* odd digit found */
                all_even = false;
                break;
            }
            temp /= 10;
        }

        if (all_even) {
            /* cast m to seq_t (search stops at the smallest such m) */
            return (seq_t)m;
        }
    }
}
