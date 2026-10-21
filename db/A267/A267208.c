/*
 * A267208  Middle column of the "Rule 109" elementary cellular automaton
 *          starting with a single ON (black) cell.
 * https://oeis.org/A267208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n == 0 or n%3 != 0; a(n) = 0 otherwise
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
    return (n % 3 == 0) ? (seq_t)0 : (seq_t)1;
}
