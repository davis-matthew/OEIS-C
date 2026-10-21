/*
 * A283234  2*A001950.
 * https://oeis.org/A283234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n*phi^2) with phi = (1+sqrt(5))/2
 * Range:     u8 n <= 3, u16 n <= 879, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes a(n) = 2 * floor( n * φ² ), where φ = (1+√5)/2.
 * This is equivalent to a(n) = 2 * ( n + floor(n*φ) ),
 * and floor(n*φ) is obtained as floor((n + floor(n*√5)) / 2).
 * The function uses integer arithmetic only, relying on the
 * floor_sqrt utility to compute floor(n*√5) exactly.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* guard against the zero offset, though the sequence starts at n=1 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* cast n to seq_t to avoid mixed types */
    seq_t sn = (seq_t)n;

    /* compute floor(n*√5) as floor(√(5*n²)) */
    seq_t n_squared = sn * sn;
    seq_t val = n_squared * (seq_t)5;
    seq_t sqrt5n = floor_sqrt(val);

    /* floor(n*φ) = floor((n + floor(n*√5))/2) */
    seq_t floor_nphi = (sn + sqrt5n) / 2;

    /* floor(n*φ²) = n + floor(n*φ) */
    seq_t floor_nphi2 = sn + floor_nphi;

    /* a(n) = 2 * floor(n*φ²) */
    seq_t result = floor_nphi2 * 2;

    return result;
}
