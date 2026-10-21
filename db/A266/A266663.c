/*
 * A266663  Total number of ON (black) cells after n iterations of the "Rule
 *          47" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = (n^2 - (-1)^n*n + 2*n + (-1)^n + 3)/2 for n > 0
 *            Simplified: if n even: a(n) = (n^2 + n + 4)/2 if n odd: a(n) =
 *            (n^2 + 3*n + 2)/2
 * Range:     u8 n <= 14, u16 n <= 344, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                 /* a(0) = 1 */

    seq_t n_seq = (seq_t)n;
    seq_t num;                    /* numerator before division by 2 */

    if ((n & 1) == 0) {           /* n is even */
        /* num = n^2 + n + 4 */
        num = n_seq * n_seq + n_seq + 4;
    } else {                      /* n is odd */
        /* num = n^2 + 3*n + 2 */
        num = n_seq * n_seq + 3 * n_seq + 2;
    }

    return num / 2;
}
