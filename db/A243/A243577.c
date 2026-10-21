/*
 * A243577  Integers of the form 8k+7 that can be written as a sum of four
 *          distinct 'almost consecutive' squares.
 * https://oeis.org/A243577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   if n mod 4 = 1: 4*n^2 + 14*n + 21 if n mod 4 = 2: 4*n^2 + 14*n +
 *            27 if n mod 4 = 3: 4*n^2 + 10*n + 21 if n mod 4 = 0: 4*n^2 +
 *            10*n + 15
 * Range:     u8 n <= 6, u16 n <= 126, u32 n <= 1728, u64 n <= 1728,
 *            u128 n <= 1728, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n & 3U;            /* n mod 4 */
    seq_t nn = (seq_t)n;
    seq_t result;

    if (r == 1U) {
        result = (seq_t)4 * nn * nn + (seq_t)14 * nn + (seq_t)21;
    } else if (r == 2U) {
        result = (seq_t)4 * nn * nn + (seq_t)14 * nn + (seq_t)27;
    } else if (r == 3U) {
        result = (seq_t)4 * nn * nn + (seq_t)10 * nn + (seq_t)21;
    } else { /* r == 0 */
        result = (seq_t)4 * nn * nn + (seq_t)10 * nn + (seq_t)15;
    }

    return result;
}
