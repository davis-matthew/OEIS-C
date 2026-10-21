/*
 * A278616  Sum of terms in level n of TRIP - Stern sequence associated with
 *          permutation triple (e,13,132).
 * https://oeis.org/A278616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = a(n‑1) + 4*a(n‑2) + a(n‑3) for n >= 3 Initial values:
 *            a(0)=3, a(1)=8, a(2)=21 Recurrence derived from the generating
 *            function (-3-5*x-x^2)/(-1+x+4*x^2+x^3) = (3+5*x+x^2)/(1 - x -
 *            4*x^2 - x^3)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)21;

    seq_t a0 = (seq_t)3;          /* a(n-3)  */
    seq_t a1 = (seq_t)8;          /* a(n-2)  */
    seq_t a2 = (seq_t)21;         /* a(n-1)  */
    seq_t an = a2;                /* placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        an = a2 + (a1 * (seq_t)4) + a0;  /* a(n-1) + 4*a(n-2) + a(n-3) */
        a0 = a1;  /* shift window: a(n-3) <- a(n-2) */
        a1 = a2;  /* a(n-2) <- a(n-1) */
        a2 = an;  /* a(n-1) <- a(n)   */
    }
    return an;
}
