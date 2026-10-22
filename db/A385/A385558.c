/*
 * A385558  Period of {binomial(N,7) mod n: N in Z}.
 * https://oeis.org/A385558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * gcd(210, n) * (2 - (n mod 2))
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* See OEIS A385558 for details. */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* For n < 1 the behaviour is undefined; we return 0. */
    if (n < 1)
        return 0;

    /* Compute g = gcd(210, n). 210 fits in seq_t. */
    seq_t g = gcd((seq_t)210, (seq_t)n);

    /* parity = 2 - (n mod 2) ; for odd n -> 1, even n -> 2   */
    seq_t parity = (seq_t)(2 - (n & 1ULL));

    /* a(n) = n * g * parity.  Multiplication order does not
       overflow for the 64‑bit build up to at least n=10 000. */
    return (seq_t)n * g * parity;
}
