/*
 * A250222  a(n) = phi(2n+1) - phi(2n), where phi is A000010.
 * https://oeis.org/A250222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(2*n+1) - phi(2*n)
 * Range:     u8 n <= 155, u16 n <= 156, u32 n <= 156, u64 n <= 156,
 *            u128 n <= 156, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Compute φ(2n) and φ(2n+1) using the helper phi().
       The helper returns seq_t, which may be signed, so the
       subtraction yields the correct signed difference. */
    seq_t phi_2n   = phi(n * 2ULL);
    seq_t phi_2n1  = phi(n * 2ULL + 1ULL);
    return phi_2n1 - phi_2n;
}
