/*
 * A267458  Number of ON (black) cells in the n-th iteration of the "Rule 133"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n/2)-1 for n≥2; a(0)=a(1)=1
 * Range:     u8 n <= 257, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n < (idx_t)2)
        return (seq_t)1;

    /* General case: 2*floor(n/2)-1 */
    seq_t half = (seq_t)(n / (idx_t)2);   /* floor division for unsigned idx_t */
    return (half * (seq_t)2) - (seq_t)1;
}
