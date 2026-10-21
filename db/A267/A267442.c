/*
 * A267442  Middle column of the "Rule 129" elementary cellular automaton
 *          starting with a single ON (black) cell.
 * https://oeis.org/A267442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A043545(n) for n > 0, a(0) = 1 a(n) = 0 iff n+1 is a
 *            power of two (except n=0), otherwise 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t x = (seq_t)(n + 1);
    return ((x & (x - 1)) == 0) ? (seq_t)0 : (seq_t)1;
}
