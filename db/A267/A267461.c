/*
 * A267461  Total number of OFF (white) cells after n iterations of the "Rule
 *          133" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^2 + 12*n - (-1)^n - 7) / 4 for n > 0; a(0) = 0
 * Range:     u8 n <= 8, u16 n <= 223, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sn   = (seq_t)n;
    seq_t term = (seq_t)2 * sn * sn + (seq_t)12 * sn;

    /* subtract (-1)^n: if n is even, (-1)^n = 1 → subtract 1;
       if n is odd,  (-1)^n = -1 → subtract (-1) = +1 */
    if ((n & (idx_t)1) == 0)
        term -= (seq_t)1;
    else
        term += (seq_t)1;

    term -= (seq_t)7;          /* final adjustment */
    return term / (seq_t)4;    /* division is exact */
}
