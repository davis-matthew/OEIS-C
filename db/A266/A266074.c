/*
 * A266074  Total number of OFF (white) cells after n iterations of the "Rule
 *          3" elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 + 2*(-1)^n*n + 6*n - (-1)^n + 1)/4
 * Range:     u8 n <= 14, u16 n <= 281, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    if ((n & 1) == 0)   /* even n: a = n*(n+4)/2 */
    {
        seq_t result = nseq * (nseq + (seq_t)4);
        result = result / 2;
        return result;
    }
    else                /* odd n: a = (n+1)^2 / 2 */
    {
        seq_t np1 = nseq + (seq_t)1;
        seq_t result = np1 * np1;
        result = result / 2;
        return result;
    }
}
