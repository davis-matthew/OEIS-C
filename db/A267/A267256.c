/*
 * A267256  Middle column of the "Rule 111" elementary cellular automaton
 *          starting with a single ON (black) cell.
 * https://oeis.org/A267256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n <= 7, otherwise a(n) = n mod 2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 8) /* first eight terms are all 1 */
        return (seq_t)1;
    /* thereafter the sequence alternates 0,1,... */
    return (seq_t)(n & 1);
}
