/*
 * A330319  a(n) = Sum_{i=1..n} phi(i)*phi(i+1), where phi(n) = A000010(n) is
 *          Euler's totient function.
 * https://oeis.org/A330319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{i=1..n} φ(i) * φ(i+1) (Euler's totient function φ)
 * Range:     u8 n <= 12, u16 n <= 83, u32 n <= 3417, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* offset starts at 1, but guard for 0 */

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t phi_i   = phi(i);
        seq_t phi_ip1 = phi(i + 1);
        sum += phi_i * phi_ip1;
    }
    return sum;
}
