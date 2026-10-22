/*
 * A383050  a(n) = Sum_{k=0..n} (k+1)^6 * Stirling1(n,k).
 * https://oeis.org/A383050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (k+1)^6 * Stirling1(n,k)
 * Range:     i8 n <= 1, i16 n <= 6, i32 n <= 11, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Binomial coefficients C[t][u] for 0 <= t,u <= 6 */
    static const int C[7][7] = {
        {1,0,0,0,0,0,0},
        {1,1,0,0,0,0,0},
        {1,2,1,0,0,0,0},
        {1,3,3,1,0,0,0},
        {1,4,6,4,1,0,0},
        {1,5,10,10,5,1,0},
        {1,6,15,20,15,6,1}
    };

    /* S[t] = sum_{k} k^t * S1(n,k)  (signed Stirling numbers of the first kind) */
    seq_t S[7];
    S[0] = 1;          /* n = 0 : S1(0,0) = 1 */
    for (int t = 1; t <= 6; ++t)
        S[t] = 0;

    if (n == 0) {
        /* a(0) = sum_{t=0}^6 C[6][t] * S[t] = 1 */
        return 1;
    }

    for (idx_t m = 1; m <= n; ++m) {
        seq_t newS[7];
        idx_t nm1 = (idx_t)(m - 1);   /* m-1 fits in idx_t; cast to seq_t below */

        for (int t = 0; t <= 6; ++t) {
            seq_t sum = 0;
            for (int u = 0; u <= t; ++u) {
                sum += S[u] * C[t][u];
            }
            sum -= (seq_t)nm1 * S[t];
            newS[t] = sum;
        }

        /* copy newS back into S for next iteration */
        for (int t = 0; t <= 6; ++t)
            S[t] = newS[t];
    }

    /* compute a(n) = sum_{t=0}^6 C[6][t] * S[t] */
    seq_t result = 0;
    for (int t = 0; t <= 6; ++t)
        result += S[t] * C[6][t];

    return result;
}
