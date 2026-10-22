/*
 * A380980  Place 2n distinct positive integers on an n-gon, n on the vertices
 *          and n on the sides such that the sums of the three values on all
 *          sides are equal. a(n) is the minimal sum of all the integers used.
 * https://oeis.org/A380980
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n*(2*n+1) + (n is even ? n/2 : 0)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 3; for other indices the result is unspecified. */
    seq_t nn = (seq_t)n;          /* cast index to value type */
    seq_t sum = nn * ((seq_t)2 * nn + (seq_t)1);  /* n*(2*n+1) */

    /* Add n/2 when n is even */
    if ((n & 1ULL) == 0ULL) {
        sum += nn / (seq_t)2;
    }

    return sum;
}
