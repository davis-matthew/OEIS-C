/*
 * A329484  Dirichlet convolution of the Liouville function with itself.
 * https://oeis.org/A329484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = tau(n) * (-1)^Omega(n), i.e., a(n) = sum_{d|n}
 *            lambda(d)*lambda(n/d) where lambda is the Liouville function
 * Range:     i8 n <= 2636, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9497 (2026-11-01)
 * Helpers:   bigomega_seq, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bigomega_seq.h"
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    /* Number of divisors of n */
    seq_t tau = (seq_t)num_divisors(n);

    /* Total number of prime factors of n (counted with multiplicity) */
    seq_t omega = bigomega_seq((seq_t)n);

    /* Sign = (-1)^omega */
    seq_t sign = (omega & 1) ? -1 : 1;

    return sign * tau;
}
