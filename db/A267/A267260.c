/*
 * A267260  Total number of ON (black) cells after n iterations of the "Rule
 *          111" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + (4-(-1)^n)*n + 4*(-1)^n)/2 for n>3; a(0)=1,
 *            a(1)=3, a(2)=6, a(3)=11
 * Range:     u8 n <= 14, u16 n <= 282, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small n: exact initial terms */
    if (n <= 3) {
        static const seq_t small[] = { (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)11 };
        return small[n];
    }

    /* for n > 3 use derived formulas avoiding negative intermediate values */
    if ((n & 1) == 0) {
        /* n even:  a(n) = (n^2 + 3n + 4)/2 */
        seq_t term = (seq_t)n * ((seq_t)n + 3) + 4;
        return term / 2;
    } else {
        /* n odd:   a(n) = (n^2 + 5n - 4)/2 */
        seq_t term = (seq_t)n * ((seq_t)n + 5);
        term -= 4;
        return term / 2;
    }
}
