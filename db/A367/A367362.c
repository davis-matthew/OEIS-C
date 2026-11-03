/*
 * A367362  Comma transform of the nonnegative integers.
 * https://oeis.org/A367362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10 * (n mod 10) + most_significant_digit(n+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9808 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t _msd_idx(idx_t x)
{
    /* Return the most significant decimal digit of x. */
    while (x >= 10) x /= 10;
    return (seq_t)x;
}

seq_t a(idx_t n)
{
    seq_t last  = (seq_t)(n % 10);
    seq_t first = _msd_idx(n + 1);  /* msd of (n+1) */
    return last * (seq_t)10 + first;
}
