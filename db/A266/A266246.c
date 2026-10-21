/*
 * A266246  Middle column of the "Rule 9" elementary cellular automaton
 *          starting with a single ON (black) cell.
 * https://oeis.org/A266246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0 or 2 or 4 or (n ≥ 5 and n is odd); otherwise
 *            a(n) = 0
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 2 || n == 4)
        return (seq_t)1;
    if (n >= 5)
        return (seq_t)(n & 1);
    return (seq_t)0;
}
