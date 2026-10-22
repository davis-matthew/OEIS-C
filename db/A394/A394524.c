/*
 * A394524  E.g.f. A(x) satisfies A(x) = 1/(1 - 4*x*A(x)^4)^(1/4).
 * https://oeis.org/A394524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{k=1}^{n-1} (8*n + 1 - 4*k) (a(0) = a(1) = 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    seq_t eight_n_plus1 = (seq_t)8 * (seq_t)n + (seq_t)1;

    for (idx_t k = (idx_t)1; k < n; ++k) {
        seq_t term = eight_n_plus1 - (seq_t)4 * (seq_t)k;
        res = res * term;
    }

    return res;
}
