/*
 * A266304  Total number of OFF (white) cells after n iterations of the "Rule
 *          15" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+(-1)^n+2)/2
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The expression is always even, so division by 2 yields an integer */
    if (n & 1ULL) /* odd n: use n*(n+1)/2 */
    {
        return ((seq_t)n * ((seq_t)n + 1)) / 2;
    }
    else          /* even n: use n*(n+3)/2 */
    {
        return ((seq_t)n * ((seq_t)n + 3)) / 2;
    }
}
