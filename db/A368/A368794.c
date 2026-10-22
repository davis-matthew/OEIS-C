/*
 * A368794  a(n) = (2*n-1)!! * Sum_{k=1..n} (-1)^(k-1)/(2*k-1)!!.
 * https://oeis.org/A368794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = (2*n - 2) * a(n-1) + (2*n - 3) *
 *            a(n-2) (n >= 2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a0 = (seq_t)0;          /* a(0) */
    seq_t a1 = (seq_t)1;          /* a(1) */
    seq_t ai = a1;                /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t c1 = (seq_t)(2 * i - 2);  /* (2*i-2) */
        seq_t c2 = (seq_t)(2 * i - 3);  /* (2*i-3) */
        ai = (a1 * c1) + (a0 * c2);
        a0 = a1;
        a1 = ai;
    }
    return ai;
}
