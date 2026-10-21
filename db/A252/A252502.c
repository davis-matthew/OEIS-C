/*
 * A252502  Number of digits of Phi_n(10), or number of digits in base b of
 *          Phi_n(b), where Phi is the cyclotomic polynomial.
 * https://oeis.org/A252502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(n) + 1 if the number of distinct prime factors of n is
 *            odd, otherwise a(n) = φ(n)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here φ(n) is Euler's totient function and the parity of the count of
   distinct primes is obtained via count_distinct_primes. */

#include "../utils/phi.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* Compute Euler's totient of n.  The utils function phi() returns seq_t. */
    seq_t ph = phi(n);

    /* Determine whether the number of distinct prime factors is odd. */
    int odd = count_distinct_primes(n) & 1;

    return ph + (seq_t)odd;
}
