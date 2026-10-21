/*
 * A243945  a(n) = Sum_{k=0..n} C(2*k, k)^2 * C(n+k, n-k).
 * https://oeis.org/A243945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*k, k)^2 * binomial(n+k, n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 gives the first term 1 directly. */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;          /* term for k = 0 */
    seq_t Ck = (seq_t)1;              /* C(2*k, k) for current k */
    seq_t Bk = (seq_t)1;              /* binomial(n+k, n-k) for current k */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update Ck : C(2k, k) = C(2(k-1), k-1) * (4k-2) / k  */
        seq_t numeratorC = Ck * ((seq_t)4 * (seq_t)k - (seq_t)2);
        Ck = numeratorC / (seq_t)k;   /* exact division */

        /* Update Bk : Bk = B(k-1) * (n+k)*(n-k+1) / ((2k)*(2k-1)) */
        seq_t two_k   = ((seq_t)k << 1);                     /* 2k */
        seq_t denom   = two_k * (two_k - (seq_t)1);          /* (2k)*(2k-1) */
        seq_t num     = Bk * ((seq_t)n + (seq_t)k) *
                       ((seq_t)n - (seq_t)k + (seq_t)1);
        Bk = num / denom;                                   /* exact */

        /* term = Ck^2 * Bk, accumulate into result */
        seq_t term = Ck * Ck;
        term = term * Bk;
        result = result + term;
    }

    return result;
}
