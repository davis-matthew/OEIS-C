/*
 * A267049  Total number of OFF (white) cells after n iterations of the "Rule
 *          91" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 + (-1)^n*(n-3) + 5) / 2 for n > 1 a(0) = 0, a(1) = 1
 * Range:     u8 n <= 15, u16 n <= 257, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    seq_t nn = (seq_t)n;
    seq_t numerator;

    /* (-1)^n is 1 if n even, -1 if n odd */
    if ((n & 1u) == 0) {
        /* n even: numerator = n^2 + n + 2 */
        numerator = nn * nn + nn + 2;
    } else {
        /* n odd:  numerator = n^2 - n + 8 */
        numerator = nn * nn - nn + 8;
    }

    return numerator / 2;
}
