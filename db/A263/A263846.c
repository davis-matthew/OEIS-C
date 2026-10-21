/*
 * A263846  Floor of cube root of n-th prime.
 * https://oeis.org/A263846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊∛(prime(n))⌋ (A048766(A000040(n)))
 * Range:     u8 n <= 18, u16 n <= 113, u32 n <= 113, u64 n <= 113,
 *            u128 n <= 113, big any n
 * Helpers:   floor_cube_root, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"          /* get_prime(idx_t) */
#include "../utils/floor_cube_root.h"    /* floor_cube_root(seq_t) */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so n>=1.  get_prime returns the n-th prime. */
    uint32_t p = get_prime(n);

    /* Convert to seq_t and compute the floor of its cube root. */
    return floor_cube_root((seq_t)p);
}
