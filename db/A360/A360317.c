/*
 * A360317  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(n-1,n-k) * binomial(2*k,k).
 * https://oeis.org/A360317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 2^(n‑k) * binomial(n-1, n-k) * binomial(2*k,
 *            k) Recurrence: n*a(n) = 2*(4*n-3)*a(n-1) - 12*(n-2)*a(n-2), with
 *            a(0)=1, a(1)=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Base values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Previous two terms: a(n-2) and a(n-1) */
    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)2;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;

        /* coefficient for a[i-1] : 2*(4*i - 3) */
        seq_t coeff1 = (seq_t)2 * ( ((seq_t)4 * i_seq) - (seq_t)3 );

        /* coefficient for a[i-2] : 12*(i - 2) */
        seq_t coeff2 = (seq_t)12 * ( i_seq - (seq_t)2 );

        seq_t term1 = coeff1 * prev1;
        seq_t term2 = coeff2 * prev2;
        seq_t numerator = term1 - term2;

        seq_t result = numerator / i_seq;  /* division is exact */

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = result;
    }
    return prev1;
}
