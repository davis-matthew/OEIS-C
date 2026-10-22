/*
 * A384346  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (4253261*s+12896793) mod 2^24 when
 *          started at s=1.
 * https://oeis.org/A384346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = (4253261 * a(n-1) + 12896793) mod 2^24 for n ≥
 *            2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* sequence values are always less than 2^24, so we keep only the low
       24 bits after each update.  The mask 0xFFFFFF works for all
       unsigned integer types that can hold 24 bits.  For arbitrary‑
       precision types such as unsigned __int128 it also works. */
    if (n == 1)
        return (seq_t)1;

    const seq_t mask = (seq_t)0xFFFFFF;
    const seq_t mul  = (seq_t)4253261;
    const seq_t add  = (seq_t)12896793;

    seq_t prev = (seq_t)1;           /* a(1) */
    idx_t i = 2;
    for (; i <= n; ++i)
    {
        prev = (mul * prev + add) & mask;
    }
    return prev;
}
