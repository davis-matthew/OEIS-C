/*
 * A385357  Consecutive states of the linear congruential pseudo-random number
 *          generator (1277*s + 24749) mod 117128 when started at s=1.
 * https://oeis.org/A385357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n) = (1277 * a(n-1) + 24749) mod 117128 for n >= 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the sequence is defined only for n >= 1 */
    if (n < (idx_t)1) return (seq_t)0; /* undefined, return 0 as a safe fallback */

    seq_t s = (seq_t)1;                /* a(1) */
    for (idx_t i = (idx_t)1; i < n; ++i) {
        s = ((seq_t)1277 * s + (seq_t)24749) % (seq_t)117128;
    }
    return s;
}
