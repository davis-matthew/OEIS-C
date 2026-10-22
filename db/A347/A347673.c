/*
 * A347673  Number of Baxter matrices of size 3 X n.
 * https://oeis.org/A347673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 9*n^3 - 16*n^2 + 6*n + 9) / 3 for n >= 3
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 14 */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)14;
    seq_t nn = (seq_t)n;
    /* Compute the numerator: n^4 + 9*n^3 - 16*n^2 + 6*n + 9 */
    seq_t num = nn * nn * nn * nn;          /* n^4 */
    num += 9 * nn * nn * nn;                /* + 9*n^3 */
    num -= 16 * nn * nn;                    /* - 16*n^2 */
    num += 6 * nn;                          /* + 6*n */
    num += 9;                               /* + 9 */
    return num / (seq_t)3;                  /* divide by 3 to get integer result */
}
