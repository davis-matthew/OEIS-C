/*
 * A267872  Number of ON (black) cells in the n-th iteration of the "Rule 237"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=5, a(3)=7; a(n)=2*a(n-1)-a(n-2) for n>3.
 *            For n≥4 this gives a(n)=2*n+1
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)        return (seq_t)1;
    if (n == 2)                  return (seq_t)5;
    if (n == 3)                  return (seq_t)7;
    /* n ≥ 4 */
    return ((seq_t)n * (seq_t)2) + (seq_t)1;
}
