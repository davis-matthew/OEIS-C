/*
 * A321883  Nonnegative integers n for which n! + 1 is not a square.
 * https://oeis.org/A321883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = all nonnegative integers except 4, 5, and 7. The index
 *            starts at 1: a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=6, etc
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    /* Count the number of valid integers encountered so far. */
    idx_t count = 0;
    /* Current integer candidate. */
    idx_t cur = 0;

    /* Loop until we have found the n-th element. */
    while (count < n) {
        /* Skip 4, 5, 7. */
        if (cur != 4 && cur != 5 && cur != 7) {
            ++count;
            if (count == n)
                break;
        }
        ++cur;
    }
    /* Return the integer as a seq_t. */
    return (seq_t)cur;
}
