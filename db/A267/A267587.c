/*
 * A267587  Middle column of the "Rule 169" elementary cellular automaton
 *          starting with a single ON (black) cell.
 * https://oeis.org/A267587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=1 for n>=5 Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=0,
 *            a(4)=0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* local table for the first five terms */
    static const seq_t first[5] = {1, 0, 1, 0, 0};

    if (n < 5)
        return first[n];
    return (seq_t)1;
}
