/*
 * A400630  Number of semi-meanders with n crossings and winding number n-4,
 *          for n >= 4.
 * https://oeis.org/A400630
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = floor((n+1)^2/2) - 10 (valid for n ≥ 4)
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the offset we return 0 (unused in formula) */
    if (n < 4) return (seq_t)0;

    /* Use seq_t for intermediate computation to avoid overflow on idx_t */
    seq_t t = (seq_t)(n + 1);
    return (t * t) / (seq_t)2 - (seq_t)10;
}
