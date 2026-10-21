/*
 * A293958  Smallest odd prime divisor of (2n+1)^2 + 1.
 * https://oeis.org/A293958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest odd prime divisor of (2n+1)^2 + 1
 * Range:     u8 n <= 5, u16 n <= 183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Compute a(n) for the OEIS sequence A293958.
 * It returns the smallest odd prime divisor of (2n+1)^2 + 1.
 */
seq_t a(idx_t n)
{
    /* (2n+1) */
    seq_t two_n_plus1 = (seq_t)n * 2 + 1;
    /* (2n+1)^2 + 1 */
    seq_t m = two_n_plus1 * two_n_plus1 + 1;
    /* m is even; remove the factor 2 to get the odd part. */
    seq_t q = m >> 1;                /* m / 2 */
    /* Find the smallest odd prime divisor of q. */
    seq_t limit = floor_sqrt(q);     /* floor(sqrt(q)) */
    for (seq_t p = 3; p <= limit; p += 2) {
        if ((q % p) == 0)
            return p;
    }
    /* If no divisor was found, q itself is prime (odd). */
    return q;
}
