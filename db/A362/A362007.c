/*
 * A362007  Fourth Lie-Betti number of a path graph on n vertices.
 * https://oeis.org/A362007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 18*n^3 - 97*n^2 + 174*n - 168)/24 for n>=4
 *            a(1)=a(2)=0, a(3)=3
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 258, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) {
        return (seq_t)0;
    }
    if (n == 3) {
        return (seq_t)3;
    }
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;          /* n^2 */
    seq_t n3 = n2 * ns;          /* n^3 */
    seq_t n4 = n3 * ns;          /* n^4 */

    /* Compute numerator as positive part minus negative part */
    seq_t pos = n4 + (seq_t)18 * n3 + (seq_t)174 * ns;   /* n^4 + 18n^3 + 174n */
    seq_t neg = (seq_t)97 * n2 + (seq_t)168;              /* 97n^2 + 168 */

    seq_t num = pos - neg;        /* guaranteed non‑negative for n>=4 */

    return num / (seq_t)24;
}
