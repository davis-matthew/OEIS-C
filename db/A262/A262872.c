/*
 * A262872  Expansion of (Sum_{i>=0} x^(i^2)) * (Sum_{j>=0} x^(j^3)) / (1-x).
 * https://oeis.org/A262872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions (x,y,z) with x +
 *            y^2 + z^3 = n. Counted by summing over all admissible y
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a simple integer cube‑root routine. */

#include <stdint.h>

/* Integer cube root: largest r with r^3 <= x */
static idx_t floor_cube_root(seq_t x)
{
    idx_t lo = 0;
    idx_t hi = 1;

    /* Increase hi until hi^3 > x */
    while ((seq_t)hi * (seq_t)hi * (seq_t)hi <= x) {
        hi <<= 1;                      /* hi *= 2 */
    }

    /* Binary search in [lo, hi) */
    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        seq_t mid3 = (seq_t)mid * (seq_t)mid * (seq_t)mid;
        if (mid3 <= x) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t n_seq = (seq_t)n;

    for (idx_t y = 0; ; ++y) {
        seq_t y2 = (seq_t)y * (seq_t)y;
        if (y2 > n_seq) {
            break;
        }
        seq_t rem = n_seq - y2;
        idx_t zmax = floor_cube_root(rem);
        count += (seq_t)(zmax + 1);  /* z from 0..zmax inclusive */
    }
    return count;
}
