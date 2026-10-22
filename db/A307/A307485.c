/*
 * A307485  A permutation of the nonnegative integers: one odd, two even, four
 *          odd, eight even, etc.; extended to nonnegative integer with a(0) =
 *          0.
 * https://oeis.org/A307485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - floor(2^k/3) where k = floor(log_2(4*n+1))
 * Range:     u8 n <= 63, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Compute floor(log2(x)) for non‑zero x.  Safe for 64‑bit values. */
static inline idx_t floor_log2_u64(idx_t x)
{
    /* x is always ≥ 1 in this context */
    idx_t r = 0;
    while (x >>= 1)
        ++r;
    return r;
}

/* Compute A307485 at index n (n ≥ 0). */
seq_t a(idx_t n)
{
    /* k = floor(log2(4*n + 1)) */
    idx_t k = floor_log2_u64(4 * n + 1);

    /* 2^k as seq_t (using the provided pow2 function for arbitrary precision) */
    seq_t pow2k = pow2(k);

    /* floor(2^k / 3) */
    seq_t div = pow2k / (seq_t)3;

    /* a(n) = 2*n - floor(2^k / 3) */
    return ((seq_t)n) * 2 - div;
}
