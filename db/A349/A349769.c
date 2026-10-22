/*
 * A349769  a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(k,floor(k/2)).
 * https://oeis.org/A349769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(k,floor(k/2))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements a direct O(n^2) computation using iterative binomial recurrences
 *  (not a search or sieve).  The function works for any index n >= 0 and
 *  is safe for arbitrary-precision seq_t and for narrow integer builds. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t cnk = 1;   /* binomial(n, k)  */
    seq_t ckm = 1;   /* binomial(k, floor(k/2))  */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* cnk = cnk * (n - k + 1) / k  (exact integer division) */
            cnk = cnk * (seq_t)(n - k + 1) / (seq_t)k;
        }

        /* term = cnk^2 * ckm */
        seq_t term = cnk * cnk * ckm;
        sum += term;

        if (k == n) break;       /* no need to update for next iteration */

        /* Update ckm to binomial(k+1, floor((k+1)/2)) */
        idx_t m = k >> 1;                    /* floor(k/2) */
        seq_t numerator   = (seq_t)(k + 1);   /* k+1   */
        seq_t denominator = (seq_t)(m + 1);   /* floor(k/2)+1 */
        ckm = ckm * numerator / denominator; /* exact integer division */
    }

    return sum;
}
