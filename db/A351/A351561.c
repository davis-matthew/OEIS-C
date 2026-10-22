/*
 * A351561  a(n) = n + d(n) - phi(n), where d is the number of divisors
 *          function, and phi is the Euler totient function.
 * https://oeis.org/A351561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + d(n) - φ(n) (offset 1)
 * Range:     u8 n <= 328, u16 n <= 9162, u32 n <= 9162, u64 n <= 9162,
 *            u128 n <= 9162, big any n
 * Helpers:   divisor_count_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* safety for lower indices, unused in tests */
        return 0;
    seq_t d  = (seq_t)divisor_count_u64(n);  /* d(n) = number of divisors of n */
    seq_t ph = phi(n);                       /* φ(n) = Euler's totient of n */
    return (seq_t)n + d - ph;
}
