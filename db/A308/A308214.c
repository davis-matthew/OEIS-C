/*
 * A308214  Sum of the smallest side lengths of all integer-sided obtuse
 *          triangles with perimeter n.
 * https://oeis.org/A308214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Output a(n) : sum of the smallest side lengths of all integer-sided
   obtuse triangles with perimeter n.  The function follows the
   definition from OEIS A308214 by direct enumeration.  */

#include <stdbool.h>          /* for bool, but not strictly needed */
#include <stdint.h>           /* uint64_t */

/* Fixed-precision integer type supplied by the build. */
extern seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* a ranges from 1 to floor(n/3) (smallest side cannot exceed one third of the perimeter). */
    for (idx_t a = 1; a <= n / 3; ++a) {
        /* For fixed a, b ranges from a to floor((n - a)/2) to keep b <= c. */
        idx_t bmax = (n - a) / 2;
        for (idx_t b = a; b <= bmax; ++b) {
            idx_t c = n - a - b;            /* largest side */
            /* Triangle inequality a + b > c */
            if (a + b <= c) continue;
            /* Obtuse condition c^2 > a^2 + b^2 */
            if ((seq_t)c * c > (seq_t)a * a + (seq_t)b * b) {
                sum += a;                   /* accumulate smallest side */
            }
        }
    }
    return sum;
}
