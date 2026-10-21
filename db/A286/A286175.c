/*
 * A286175  Sum of the n-th entries in all cycles of all permutations of
 *          [n+1].
 * https://oeis.org/A286175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n-1)! * ((n+1)(n+2)/2 - 1) + (n-2)! * ((n+1)(n+2)/2 -
 *            3)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the offset: the sequence is defined for n >= 1 */
    if (n == 0) return 0;

    /* Compute (n-1)! */
    seq_t fact1 = 1;
    for (idx_t i = 1; i < n; ++i) {
        fact1 *= (seq_t)i;
    }

    /* Compute (n-2)! if needed */
    seq_t fact2 = 1;
    if (n >= 2) {
        for (idx_t i = 1; i < n - 1; ++i) {
            fact2 *= (seq_t)i;
        }
    }

    /* Compute S1 and S2 */
    seq_t up1 = (seq_t)(n + 1);   /* n+1 */
    seq_t up2 = (seq_t)(n + 2);   /* n+2 */

    seq_t S1 = (up1 * up2 - (seq_t)2) / (seq_t)2;   /* sum_{k=2}^{n+1} k */
    seq_t S2 = (up1 * up2 - (seq_t)6) / (seq_t)2;   /* sum_{k=3}^{n+1} k */

    /* Compute the result */
    seq_t result = (seq_t)2 * fact1 * S1;
    if (n >= 2) {
        result += fact2 * S2;
    }

    return result;
}
