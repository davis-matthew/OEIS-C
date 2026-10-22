/*
 * A316457  Expansion of x*(31 + 326*x + 336*x^2 + 26*x^3 + x^4) / (1 - x)^6.
 * https://oeis.org/A316457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^5 + 15*n^4 + 10*n^3
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 58, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (so a(1) is the first term) */
seq_t a(idx_t n)
{
    /* n is a non‑negative unsigned integer (offset 1). */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;   /* n^2 */
    seq_t n3 = n2 * nn;   /* n^3 */
    seq_t n4 = n3 * nn;   /* n^4 */
    seq_t n5 = n4 * nn;   /* n^5 */

    return (seq_t)6 * n5 + (seq_t)15 * n4 + (seq_t)10 * n3;
}
