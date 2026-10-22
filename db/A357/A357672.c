/*
 * A357672  a(n) = Sum_{k = 0..n} binomial(n+k-1,k) * Sum_{k = 0..n}
 *          binomial(n+k-1,k)^2.
 * https://oeis.org/A357672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} C(n+k-1,k) * Sum_{k=0..n} C(n+k-1,k)^2
 *            (equivalently a(n) = A000984(n) * A333592(n))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum1 = (seq_t)1;      /* sum of C(n+k-1,k) over k=0..n */
    seq_t sum2 = (seq_t)1;      /* sum of C(n+k-1,k)^2 over k=0..n */
    seq_t c    = (seq_t)1;      /* current binomial value C(n+k-1,k) */

    for (idx_t k = 1; k <= n; ++k) {
        /* c = C(n+k-1, k) = C(n+k-2, k-1) * (n+k-1)/k */
        c = c * ((seq_t)(n + k - 1)) / (seq_t)k;
        sum1 += c;
        sum2 += c * c;
    }

    return sum1 * sum2;
}
