/*
 * A381022  Expansion of e.g.f. -log(1-x)^3 * exp(x) / 6.
 * https://oeis.org/A381022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * |Stirling1(k,3)|
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A381022 for all n ≥ 0. */

seq_t a(idx_t n)
{
    seq_t result = 0;            /* final answer */
    seq_t binom  = 1;            /* C(n,0)  */
    seq_t s1 = 0, s2 = 0, s3 = 0;/* |Stirling1(k,1)|, |Stirling1(k,2)|, |Stirling1(k,3)| for current k */

    for (idx_t k = 0; k <= n; ++k) {
        /* add the current term if k ≥ 3 */
        if (k >= 3) {
            result += binom * s3;
        }

        /* update Stirling numbers for k+1 (unless we are at the last k) */
        if (k < n) {
            seq_t next_s1 = (k == 0) ? (seq_t)1                : s1 * (seq_t)k;
            seq_t next_s2 = s1 + (seq_t)k * s2;
            seq_t next_s3 = s2 + (seq_t)k * s3;

            s1 = next_s1;
            s2 = next_s2;
            s3 = next_s3;

            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1); /* C(n,k+1) from C(n,k) */
        }
    }

    return result;
}
