/*
 * A358061  a(n) = phi(n) mod tau(n).
 * https://oeis.org/A358061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n) mod tau(n)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   divisor_count_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Compute Euler's totient of n.  phi(n) is provided by the utils
       library and returns a value of type seq_t.                        */
    seq_t phi_val = phi(n);

    /* Compute the number of divisors of n (tau(n)).  For an
       idx_t argument we use divisor_count_u64 from the utils
       library; it returns a 64‑bit unsigned integer which we cast to
       seq_t for compatibility with phi_val.                         */
    seq_t tau = (seq_t)divisor_count_u64((uint64_t)n);

    /* Return phi(n) modulo tau(n).  The modulo operator works for
       all supported seq_t types (built‑in unsigned types and the
       arbitrary‑precision integer class).                            */
    return phi_val % tau;
}
