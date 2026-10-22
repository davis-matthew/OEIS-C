/*
 * A346896  Expansion of e.g.f.: (1-12*x)^(-11/12).
 * https://oeis.org/A346896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0}^{n-1} (12*k + 11)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently a(n) = 12^n * Γ(n+11/12)/Γ(11/12)) */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 0; k < n; ++k)
    {
        /* compute the factor (12*k + 11) in the proper type */
        seq_t factor = (seq_t)12 * (seq_t)k + (seq_t)11;
        res *= factor;
    }
    return res;
}
