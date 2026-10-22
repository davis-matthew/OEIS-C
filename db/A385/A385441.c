/*
 * A385441  E.g.f. A(x) satisfies A(x) = exp( arcsinh(x * A(x)^3) ).
 * https://oeis.org/A385441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=0}^{n-2} (2*(n + k) + 3), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    for (idx_t i = 0; i <= n - 2; ++i)
    {
        seq_t term = ((seq_t)2 * ((seq_t)n + (seq_t)i)) + 3;
        res = res * term;
    }
    return res;
}
