/*
 * A322029  Denominator of least value of the squared diameters of the
 *          enclosing circles of all strictly convex lattice n-gons with
 *          minimal area given by A070911. Numerators are A321693.
 * https://oeis.org/A322029
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, frac, hard
 * Formula:   a(n) = p² if n−1 is a power of two and p is the largest prime <
 *            n−1; otherwise a(n) = 1. For n < 3 the behavior is irrelevant
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* offset starts at n = 3 */
    idx_t m = n - 1;              /* m = n−1 */

    /* Check if m is a power of two (except 1). */
    if ((m & (m - 1)) != 0) {
        return 1;
    }

    /* Find the largest prime less than m. */
    for (idx_t p = m - 1; p >= 2; --p) {
        if (is_prime(p)) {
            return (seq_t)p * (seq_t)p;
        }
    }

    /* No prime found (happens for n = 3). */
    return 1;
}
