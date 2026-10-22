/*
 * A352077  a(n) = floor( Sum_{k=1..n} k^(1/3) ).
 * https://oeis.org/A352077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( Sum_{k=1..n} k^(1/3) )
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the sum using high‑precision fixed‑point arithmetic:
   choose a scaling factor S > n, compute each term as floor( (k*S³)^(1/3) ),
   accumulate, then divide by S. For n ≤ 10000 S = 10001 is safe
   because the accumulated error is < n/S < 1, so the final floor is exact. */
#include <stdlib.h>
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* scaling factor S must be > n to bound the error below 1 */
    const seq_t S = (seq_t)10001;
    /* pre‑compute S³ */
    seq_t S3 = S * S;
    S3 = S3 * S;

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        /* M = k * S³ */
        seq_t M = (seq_t)k * S3;
        /* floor of cube root of M gives floor( k^(1/3) * S ) */
        seq_t term = floor_cube_root(M);
        sum += term;
    }
    /* result = floor( sum / S ) */
    return sum / S;
}
