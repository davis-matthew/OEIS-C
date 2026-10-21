/*
 * A274233  Number of partitions of n*(n-1)/2 into at most three parts.
 * https://oeis.org/A274233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( ((n*(n-1)/2)^2 + 6*(n*(n-1)/2) + 12) / 12 )
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 426, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n*(n-1)/2 into at most three parts. */
seq_t a(idx_t n)
{
    /* Compute t = n*(n-1)/2 safely in seq_t */
    seq_t t = (seq_t)n * (n - 1) / 2;
    /* Compute the quadratic expression and divide by 12.  All intermediate
       values are non‑negative, so ordinary truncating integer division
       yields the floor. */
    return (t * t + 6 * t + 12) / 12;
}
