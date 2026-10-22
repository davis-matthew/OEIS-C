/*
 * A306519  Expansion of 2/(1 + 2*x + sqrt(1 - 4*x*(1 + x))).
 * https://oeis.org/A306519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} (-1)^(n-k) * binomial(n,k) * C(2k,k) where
 *            C(2k,k) are central binomial coefficients. Equivalent 4‑term
 *            recurrence: (n+1)·a(n) – 3(n−1)·a(n−1) – 2(4n−5)·a(n−2) –
 *            4(n−2)·a(n−3) = 0 Initial values: a(0)=1, a(1)=0, a(2)=2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    /* a(n-3), a(n-2), a(n-1) for the current iteration */
    seq_t a_n_3 = (seq_t)1;   /* a(0) */
    seq_t a_n_2 = (seq_t)0;   /* a(1) */
    seq_t a_n_1 = (seq_t)2;   /* a(2) */
    seq_t a_n;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t term1 = 3 * (seq_t)(i - 1) * a_n_1;          /* 3*(i-1)*a(n-1) */
        seq_t term2 = 2 * (seq_t)(4 * i - 5) * a_n_2;      /* 2*(4i-5)*a(n-2) */
        seq_t term3 = 4 * (seq_t)(i - 2) * a_n_3;          /* 4*(i-2)*a(n-3) */
        seq_t numerator = term1 + term2 + term3;
        a_n = numerator / ((seq_t)(i + 1));                /* divide by (i+1) */

        /* shift window */
        a_n_3 = a_n_2;
        a_n_2 = a_n_1;
        a_n_1 = a_n;
    }
    return a_n_1;  /* a(n) */
}
