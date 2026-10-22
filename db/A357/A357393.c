/*
 * A357393  E.g.f. satisfies A(x) = -log(1 - x * exp(3 * A(x))).
 * https://oeis.org/A357393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n-1)!/(3*n)! for n>0 (empty product = 1), a(0)=0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = product_{k=3*n+1}^{4*n-1} k */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t res = 1;
    for (idx_t k = 3*n + 1; k < 4*n; ++k)
        res *= (seq_t)k;
    return res;
}
