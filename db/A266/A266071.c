/*
 * A266071  Binary representation of the middle column of the "Rule 3"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = floor(991*10^n/990) = 10^n + floor(10^n/990)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow10 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow10 *= (seq_t)10;

    seq_t add = pow10 / (seq_t)990;
    return pow10 + add;
}
