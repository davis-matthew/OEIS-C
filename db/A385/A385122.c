/*
 * A385122  a(n) = d(phi(n)) - phi(d(n)) where d(n) = A000005(n) is the number
 *          of divisors and phi(n) = A000010(n) is the Euler totient function.
 * https://oeis.org/A385122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = d(phi(n)) - phi(d(n)) where d(n) = A000005(n) is the
 *            number of divisors and phi(n) = A000010(n) is Euler's totient
 *            function
 * Range:     i8 n <= 349, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* phi(n) */
    seq_t phi_n = phi(n);

    /* number of divisors of phi(n) */
    seq_t d_phi_n = (seq_t)divisor_count_u64((uint64_t)phi_n);

    /* d(n) */
    seq_t d_n = (seq_t)divisor_count_u64((uint64_t)n);

    /* phi(d(n)) */
    seq_t phi_d_n = phi((idx_t)d_n);

    return d_phi_n - phi_d_n;
}
