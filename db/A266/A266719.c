/*
 * A266719  Middle column of the "Rule 59" elementary cellular automaton
 *          starting with a single ON (black) cell.
 * https://oeis.org/A266719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 0, a(2) = 1, a(n) = 0 for even n ≥ 4, a(n) = 1
 *            for odd n ≥ 3
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    /* For n ≥ 3, return 1 if n is odd, otherwise 0. */
    return ((n & 1) ? (seq_t)1 : (seq_t)0);
}
