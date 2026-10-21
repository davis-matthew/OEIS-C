/*
 * A274719  Expansion of Product_{k >= 1} (1-q^(2*k)).
 * https://oeis.org/A274719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1, if n = 0; a(n) = (-1)^k, if n = k(3k-1) or n = k(3k+1)
 *            for some k >= 1; a(n) = 0 otherwise. (This follows from
 *            ∏_{k>=1}(1-q^{2k}) expanding as the Euler pentagonal theorem
 *            with q→q².)
 * Range:     i8 n <= 9720, i16 n <= 9720, i32 n <= 9720, i64 n <= 9720,
 *            i128 n <= 9720, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* search for k such that n = k(3k-1) or n = k(3k+1) */
    for (idx_t k = 1; ; ++k) {
        /* compute the two pentagonal exponents */
        idx_t e1 = k * (3 * k - 1);
        idx_t e2 = k * (3 * k + 1);

        /* once both exceed n we can stop */
        if (e1 > n && e2 > n)
            break;

        if (e1 == n || e2 == n) {
            /* sign = (-1)^k */
            if (k & 1)
                return (seq_t)-1;
            else
                return (seq_t)1;
        }
    }
    return (seq_t)0;              /* a(n) = 0 for all other n */
}
