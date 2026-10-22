/*
 * A378404  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+3*k,n-k).
 * https://oeis.org/A378404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+3*k,n-k)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t c1   = 1;          /* C(n,0)   */
    seq_t c2   = 1;          /* C(n, n)  */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* add current term to the sum */
        sum += c1 * c2;

        if (k == n)
            break;          /* done, avoid overflow for next step */

        /* update binomial(n, k+1) from binomial(n, k) */
        c1 = c1 * ((seq_t)n - k) / (k + 1);

        /* update binomial(n+3k, n-k) to binomial(n+3(k+1), n-(k+1)) */
        seq_t a_val = (seq_t)n + 3 * k;       /* a = n + 3k           */
        seq_t b_val = (seq_t)n - k;           /* b = n - k           */
        /* compute the ratio */
        seq_t num = (a_val + 1) * (a_val + 2) * (a_val + 3) * b_val;
        seq_t den = (a_val - b_val + 1) * (a_val - b_val + 2) *
                    (a_val - b_val + 3) * (a_val - b_val + 4);
        c2 = c2 * num / den;
    }

    return sum;
}
