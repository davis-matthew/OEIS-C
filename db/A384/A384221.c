/*
 * A384221  Consecutive states of the linear congruential pseudo-random number
 *          generator for the Texas Instruments TI99 when started at 1.
 * https://oeis.org/A384221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = (28645*a(n-1) + 31417) mod 2^16 for n>1
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* behavior for indices below offset irrelevant */

    seq_t x = 1;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        x = (28645 * x + 31417) & (seq_t)0xFFFF;
    }
    return x;
}
