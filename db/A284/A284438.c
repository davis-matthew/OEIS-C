/*
 * A284438  a(n) = phi(n)^n.
 * https://oeis.org/A284438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n)^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   phi, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t phi_n = phi(n);
    return seq_pow(phi_n, n);
}
