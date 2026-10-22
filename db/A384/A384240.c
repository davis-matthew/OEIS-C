/*
 * A384240  Consecutive states of the linear congruential pseudo-random number
 *          generator (2897*s + 1) mod 2^23 when started at s=1.
 * https://oeis.org/A384240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2897 * a(n-1) + 1) mod 2^23, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Mask for modulo 2^23 (i.e., 0x7FFFFF). */
    const seq_t mask = ((((seq_t)1) << 23) - 1);

    /* The sequence starts with a(1) = 1. */
    seq_t val = 1;

    if (n <= 1)
        return val;

    /* Compute iteratively up to the requested index. */
    for (idx_t i = 2; i <= n; ++i)
    {
        val = (2897 * val) + 1;
        val &= mask;    /* equivalent to modulo 2^23 for unsigned types */
    }
    return val;
}
