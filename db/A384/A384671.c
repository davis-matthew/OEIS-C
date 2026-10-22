/*
 * A384671  Expansion of (1-x^2) / (1-2*x-5*x^2+2*x^3).
 * https://oeis.org/A384671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2) - 2*a(n-3) for n >= 3; a(0)=1,
 *            a(1)=2, a(2)=8
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;

    /* Roll three recent values to avoid storing the whole sequence. */
    seq_t prev3 = (seq_t)1;   /* a(0) */
    seq_t prev2 = (seq_t)2;   /* a(1) */
    seq_t prev1 = (seq_t)8;   /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = prev1 * (seq_t)2 + prev2 * (seq_t)5 - prev3 * (seq_t)2;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
