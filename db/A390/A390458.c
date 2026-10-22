/*
 * A390458  Minimum site perimeter of a polyiamond with n cells.
 * https://oeis.org/A390458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(6*n+2)) - [6*n is a perfect square]
 * Range:     u8 n <= 10, u16 n <= 2618, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Compute a(n) for the Minimum Site Perimeter of a Polyiamond with n cells.
 * The function is fully self‑contained and uses only integer arithmetic
 * and the integer square root routine from the utils library. */
seq_t a(idx_t n) {
    /* Avoid underflow for the very first term by assuming n>=1; the caller
     * guarantees offset 1. */
    seq_t six = (seq_t)6;
    seq_t two = (seq_t)2;

    /* t6 = 6 * n */
    seq_t t6 = six * (seq_t)n;
    /* x  = 6 * n + 2 */
    seq_t x  = t6 + two;

    /* Ceil square root of x: ceil_sqrt = floor_sqrt(x) + (floor_sqrt(x)^2 != x) */
    seq_t fs_x = floor_sqrt(x);
    seq_t ceil_sqrt = fs_x;
    if (fs_x * fs_x != x) {
        ceil_sqrt += (seq_t)1;
    }

    /* Subtract 1 if 6*n is itself a perfect square */
    seq_t fs_t6 = floor_sqrt(t6);
    if (fs_t6 * fs_t6 == t6) {
        ceil_sqrt -= (seq_t)1;
    }

    return ceil_sqrt;
}
