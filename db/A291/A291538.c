/*
 * A291538  a(n) = PrimePi(n^3) - PrimePi(n)^3, where PrimePi = A000720.
 * https://oeis.org/A291538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = PrimePi(n^3) - PrimePi(n)^3 (offset 1)
 * Range:     u8 n <= 14, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* π(n) */
    seq_t pi_n = prime_pi(n);

    /* n^3 (fits in idx_t for all n considered) */
    idx_t n3 = n * n * n;

    /* π(n^3) */
    seq_t pi_n3 = prime_pi(n3);

    /* π(n)^3 */
    seq_t pi_n_cub = pi_n * pi_n * pi_n;

    return pi_n3 - pi_n_cub;
}
