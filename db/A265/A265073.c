/*
 * A265073  Coordination sequence for (3,3,6) tiling of hyperbolic plane.
 * https://oeis.org/A265073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 47, u64 n <= 97, u128 n <= 198,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence A265073 for the (3,3,6) tiling of the hyperbolic plane.
   The sequence is given by the rational generating function

      G(x) = (x³+1)(x²+x+1)(x+1) / (x⁶ - x⁵ - x⁴ + x³ - x² - x + 1)

   From the denominator we obtain the linear recurrence (for the auxiliary
   sequence b(n) = a(n) for n ≥ 1 and b(0) = 0):

      b(n) = b(n-1) + b(n-2) - b(n-3) + b(n-4) + b(n-5) - b(n-6)   (n ≥ 6)

   With the initial values
      b(0) = 0,  b(1) = 3,  b(2) = 6,  b(3) = 10,  b(4) = 16,  b(5) = 26
   we can compute all terms.  For n = 0 the value is a(0) = 1 (outside the
   recurrence).  This implementation uses a circular buffer of the last six
   values to achieve O(n) time and O(1) additional space. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for the offset. */
    if (n == 0)
        return (seq_t)1;  /* a(0) = 1 */

    /* Circular buffer for the last six values of the auxiliary sequence b. */
    seq_t buf[6] = {
        (seq_t)0, /* b(0) */
        (seq_t)3, /* b(1) */
        (seq_t)6, /* b(2) */
        (seq_t)10,/* b(3) */
        (seq_t)16,/* b(4) */
        (seq_t)26 /* b(5) */
    };

    /* If the requested index is within the initial data – return directly. */
    if (n <= 5)
        return buf[n];

    /* Compute subsequent terms using the recurrence. */
    seq_t next = 0;
    for (idx_t i = 6; i <= n; ++i) {
        /* b(i) = b(i-1) + b(i-2) - b(i-3) + b(i-4) + b(i-5) - b(i-6) */
        next = buf[5] + buf[4] - buf[3] + buf[2] + buf[1] - buf[0];

        /* Shift the circular buffer for the next iteration. */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }

    /* For n ≥ 1 the sequence a(n) equals b(n). */
    return next;
}
