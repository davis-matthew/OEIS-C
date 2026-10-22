/*
 * A391594  a(n) = Sum_{k=0..2*n} binomial(2*k,2*n-k).
 * https://oeis.org/A391594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2) + a(n-3); Initial values: a(0) = 1,
 *            a(1) = 3, a(2) = 13
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)13;

    seq_t a0 = (seq_t)1, a1 = (seq_t)3, a2 = (seq_t)13, ai;
    for (idx_t i = 3; i <= n; ++i) {
        ai = (seq_t)5 * a2 + a0 - (seq_t)2 * a1;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return ai;
}
