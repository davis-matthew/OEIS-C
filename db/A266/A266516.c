/*
 * A266516  Decimal representation of the n-th iteration of the "Rule 29"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n for even n; a(n) = 2*(4^n - 2^n) - 1 for odd n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) /* even n */
        return (seq_t)1 << n;

    /* odd n: 4^n = 2^(2n) */
    seq_t a4 = (seq_t)1 << (n << 1);
    seq_t a2 = (seq_t)1 << n;
    seq_t res = (a4 - a2) << 1;
    res -= 1;
    return res;
}
