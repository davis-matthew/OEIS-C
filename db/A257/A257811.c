/*
 * A257811  Circle of fifths cycle (clockwise).
 * https://oeis.org/A257811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 7*(n-1) mod 12
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset starts at 1; for safety return 0 if n==0 */
    if (n == 0)
        return (seq_t)0;

    idx_t k   = (n - 1) % (idx_t)12;   /* (n-1) modulo 12 */
    idx_t val = (k * (idx_t)7) % (idx_t)12; /* 7*(n-1) modulo 12 */
    return (seq_t)(val + (idx_t)1);      /* +1 to shift from 0-based to 1-based */
}
