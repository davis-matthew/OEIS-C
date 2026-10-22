/*
 * A318774  Coefficients in expansion of 1/(1 - x - 3*x^4).
 * https://oeis.org/A318774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-4); a(0) = a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 45, u64 n <= 89, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly. */
    if (n < 4)
        return (seq_t)1;

    /* Keep only the last four values; the array size is fixed (4). */
    seq_t buf[4] = { (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1 };

    /* Compute successive terms up to n using the recurrence. */
    for (idx_t i = 4; i <= n; ++i)
    {
        /* i%4 overwrites the oldest value (a(i-4)).  The other
           references use the previous values needed for the
           recurrence. */
        buf[i % 4] = buf[(i - 1) % 4] + (seq_t)3 * buf[(i - 4) % 4];
    }

    /* The value of a(n) now resides at index n%4. */
    return buf[n % 4];
}
