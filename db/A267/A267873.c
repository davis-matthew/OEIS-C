/*
 * A267873  Number of ON (black) cells in the n-th iteration of the "Rule 235"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n = 0 or 1)
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            4  (n = 2) */
/*            2*n + 1  (n >= 3) */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)4;
    seq_t res = (seq_t)n;
    res = res * (seq_t)2 + (seq_t)1;
    return res;
}
