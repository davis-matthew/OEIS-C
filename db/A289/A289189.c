/*
 * A289189  Upper bound for certain restricted sumsets.
 * https://oeis.org/A289189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (sqrt(4*n + 9) + 3) / 2 )
 * Range:     u8 n <= 61, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: returns floor(sqrt(x)) for unsigned integer seq_t. */
static seq_t
isqrt_seq(seq_t x)
{
    seq_t low  = 0;
    seq_t high = 1;

    /* Find an upper bound for sqrt(x) by doubling until high > sqrt(x). */
    while (high <= x / high) {
        high <<= 1;
    }

    /* Binary search between low and high. */
    while (low < high) {
        seq_t mid = low + (high - low + 1) / 2;
        if (mid <= x / mid) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Compute t = 4*n + 9.  Use seq_t for all intermediate calculations. */
    seq_t t = (seq_t)4 * (seq_t)n + (seq_t)9;

    /* Compute floor(sqrt(t)). */
    seq_t r = isqrt_seq(t);

    /* Return floor((r + 3)/2). */
    return (r + (seq_t)3) / (seq_t)2;
}
