/*
 * A255437  In positive integers: replace k^2 with the first k odd numbers.
 * https://oeis.org/A255437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by scanning natural numbers m, replacing each
 *            square k^2 with the first k odd numbers. For a given index n we
 *            find the minimal m such that C(m) = m + ⌊√m⌋(⌊√m⌋−1)/2 ≥ n,
 *            where C(m) is the cumulative length of the sequence up to and
 *            including the block for m. If m is a perfect squa...
 * Range:     u8 n <= 375, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Integer square root: floor(sqrt(n)) using binary search. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;          /* sqrt(2^64-1) < 2^32 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* Cumulative length up to natural number m. */
static inline seq_t C(idx_t m)
{
    idx_t s = isqrt_u64(m);
    seq_t part1 = (seq_t)m;
    seq_t part2 = (seq_t)s * ((seq_t)s - 1) / 2;
    return part1 + part2;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out-of-range call */

    /* Binary search for minimal m with C(m) ≥ n. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (C(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Position within the block for m. */
    idx_t s = isqrt_u64(m);
    seq_t prevC = C(m - 1);
    seq_t offset = (seq_t)n - prevC;          /* 1‑based offset within the block */

    if (s * s == m) {                         /* m is a perfect square k² */
        return 2 * offset - 1;                /* j‑th odd number */
    } else {
        return (seq_t)m;                      /* the number itself */
    }
}
