/*
 * A316317  Coordination sequence for trivalent node in chamfered version of
 *          square grid.
 * https://oeis.org/A316317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n > 0 : a(n + 12) = a(n) + 40 This yields a(n) =
 *            base[(n-1)%12 + 1] + 40 * ((n-1)/12) (n > 0)
 * Range:     u8 n <= 77, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ensure bool type available for any helper code (not strictly needed here) */
#include <stdbool.h>

/* Base values for a(1) through a(12) */
static const seq_t base[13] = {
    0,   /* unused index 0 */
    3,   /* a(1)  */
    6,   /* a(2)  */
   11,   /* a(3)  */
   14,   /* a(4)  */
   15,   /* a(5)  */
   20,   /* a(6)  */
   25,   /* a(7)  */
   26,   /* a(8)  */
   29,   /* a(9)  */
   34,   /* a(10) */
   37,   /* a(11) */
   40    /* a(12) */
};

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute quotient and remainder for (n-1) with modulus 12 */
    idx_t q = (n - 1) / 12;
    idx_t r = ((n - 1) % 12) + 1;        /* r in 1..12 */

    /* Calculate a(n) using the recurrence */
    seq_t result = base[r] + (seq_t)40 * (seq_t)q;
    return result;
}
