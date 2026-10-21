/*
 * A267878  Middle column of the "Rule 233" elementary cellular automaton
 *          starting with a single ON (black) cell.
 * https://oeis.org/A267878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for all n except a(1) = 0 and a(3) = 0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n == 1 || n == 3) ? (seq_t)0 : (seq_t)1;
}
