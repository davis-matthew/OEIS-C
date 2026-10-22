/*
 * A369669  The greatest common divisor of the first and the second arithmetic
 *          derivative of n.
 * https://oeis.org/A369669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A003415(n), A068346(n)) (i.e. greatest common divisor
 *            of the first and second arithmetic derivatives). Equivalent for
 *            n≥2: a(n) = A085731(A003415(n))
 * Range:     u8 n <= 159, u16 n <= 9282, u32 n <= 9282, u64 n <= 9282,
 *            u128 n <= 9282, big any n
 * Helpers:   arith_derivative_uint64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/arith_derivative_uint64.h"

/* a(n) = gcd(first derivative, second derivative) */
seq_t a(idx_t n)
{
    /* for 0 and 1 the derivatives are defined to be 0 */
    if (n < 2)
        return 0;

    /* first derivative */
    seq_t d1 = arith_derivative_uint64((uint64_t)n);

    /* second derivative: derivative of d1 (which is a 64‑bit integer) */
    seq_t d2 = arith_derivative_uint64((uint64_t)d1);

    /* gcd of the two derivatives */
    return gcd(d1, d2);
}
