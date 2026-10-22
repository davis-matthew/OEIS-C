/*
 * A306277  Numbers congruent to 1 or 8 mod 10.
 * https://oeis.org/A306277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 5*n - (2 + 2*(n%2)) (for n ≥ 1, OEIS offset 1)
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If the caller passes n == 0 (offset not defined) return 0 or any value. */
    if (n == 0) return (seq_t)0;

    seq_t nn = (seq_t)n;                     /* cast index to seq_t */
    /* nn*5 is always ≥ 5; subtract 2 for even n, 4 for odd n. */
    return nn * 5 - ((seq_t)2 + ((nn & (seq_t)1) << 1));
}
