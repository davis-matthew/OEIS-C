/*
 * A246934  The closest square to n-th prime.
 * https://oeis.org/A246934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest perfect square to the n-th prime
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* out of offset – any value is acceptable */

    /* Retrieve the n‑th prime.  get_prime returns a 32‑bit unsigned integer. */
    uint32_t p32 = get_prime(n);
    seq_t   p   = (seq_t)p32;

    /* Integer floor of the square root of p. */
    seq_t k   = floor_sqrt(p);

    seq_t sq1 = k * k;                 /* k²  */
    seq_t k2  = k + (seq_t)1;           /* k+1 */
    seq_t sq2 = k2 * k2;                /* (k+1)² */

    seq_t diff1 = p - sq1;              /* p – k²  (non‑negative) */
    seq_t diff2 = sq2 - p;              /* (k+1)² – p (non‑negative) */

    return (diff1 <= diff2) ? sq1 : sq2;
}
