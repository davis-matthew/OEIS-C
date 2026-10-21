/*
 * A266665  Total number of OFF (white) cells after n iterations of the "Rule
 *          47" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; a(n) = n*(n+1)/2 , for odd n > 0 a(n) = (n*n + 3*n -
 *            2)/2 , for even n > 0 (These are equivalent to the conjectured
 *            formula a(n) = (n^2+2*n+(-1)^n*(n-1)-1)/2 for n>0.)
 * Range:     u8 n <= 15, u16 n <= 289, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sn = (seq_t)n;

    if (n & 1U) {                      /* odd n */
        /* a(n) = n*(n+1)/2 */
        seq_t prod = sn * (sn + 1);
        return prod / 2;
    } else {                           /* even n */
        /* a(n) = (n*n + 3*n - 2)/2 */
        seq_t term1 = sn * sn;         /* n^2 */
        seq_t term2 = 3 * sn;          /* 3*n */
        seq_t numerator = term1 + term2 - 2;
        return numerator / 2;
    }
}
