/*
 * A283028  Number of inequivalent 4 X 4 matrices with entries in
 *          {1,2,3,...,n} up to vertical and horizontal reflections.
 * https://oeis.org/A283028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 * (n^8 + 3)/4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 17, u128 n <= 279,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute n^8 */
    seq_t nn = (seq_t)n;
    seq_t power = 1;
    for (int i = 0; i < 8; ++i)
        power *= nn;

    /* n even: n^8 is divisible by 4
       n odd:  n^8 + 3 is divisible by 4   */
    if ((n & 1) == 0) {
        seq_t div4 = power / (seq_t)4;
        return div4 * (power + (seq_t)3);
    } else {
        seq_t div4 = (power + (seq_t)3) / (seq_t)4;
        return power * div4;
    }
}
