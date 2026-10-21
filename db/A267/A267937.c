/*
 * A267937  Binary representation of the n-th iteration of the "Rule 251"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 101, a(n) for n ≥ 2 is the decimal integer
 *            consisting of 2n+1 consecutive digits 1, i.e. a(n) = (10^(2n+1)
 *            – 1) / 9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)101;

    /* number of decimal digits (all '1') */
    idx_t w = 2 * n + 1;

    seq_t val = 0;
    for (idx_t i = 0; i < w; ++i)
    {
        val = val * (seq_t)10 + (seq_t)1;
    }
    return val;
}
