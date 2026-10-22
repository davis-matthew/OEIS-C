/*
 * A365667  Expansion of Sum_{0<i<j<k<l<m} q^(2*(i+j+k+l+m)-5)/(
 *          (1-q^(2*i-1))*(1-q^(2*j-1))*(1-q^(2*k-1))*(1-q^(2*l-1))*(1-q^(2*m-1))
 *          )^2.
 * https://oeis.org/A365667
 *
 * Offset:    25    (a(25) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in G(q) = -(1/5) * ( Σ_{k≥5} (-1)^k *
 *            k * binomial(k+4,9) * q^{k^2} ) / ( 1 + 2 * Σ_{k≥1} (-q)^{k^2} )
 * Range:     u8 n <= 35, u16 n <= 55, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: 1. Build N[i] = (-1)^k * k * binomial(k+4,9) for i = k^2 (k≥5).
   2. Recurrence for S[i] such that D * S = N, where D[0]=1 and
      D[sq] = 2*(-1)^sqrt(sq) for sq a perfect square.
       S[i] = N[i] - Σ_{sq≤i} D[sq] * S[i-sq].
   3. a(n) = -(S[n]/5).  */

#include <math.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 25) return 0;                           /* offset is 25 */

    size_t N = (size_t)n;
    size_t len = N + 1;

    long long *S = (long long *)malloc(len * sizeof(long long));
    long long *Narr = (long long *)malloc(len * sizeof(long long));
    if (!S || !Narr) {
        free(S); free(Narr);
        return 0;
    }

    for (size_t i = 0; i < len; ++i) Narr[i] = 0;

    /* --- fill N array with non‑zero terms k^2 (k≥5) --- */
    for (idx_t k = 5; (size_t)(k * k) <= N; ++k) {
        size_t sq = (size_t)(k * k);
        /* compute binomial(k+4, 9) */
        long long binom = 1;
        idx_t top = k + 4;
        for (idx_t i = 1; i <= 9; ++i) {
            binom = (binom * (top - 9 + i)) / i;
        }
        long long sign = (k % 2 == 0) ? 1 : -1;
        Narr[sq] = sign * (long long)k * binom;
    }

    /* --- compute S[i] by recurrence --- */
    S[0] = 0;    /* N[0] == 0 */
    for (size_t i = 1; i <= N; ++i) {
        long long sum = 0;
        int limit = (int) sqrt((double)i);          /* floor sqrt(i) */
        for (int t = 1; t <= limit; ++t) {
            size_t sq = (size_t)(t * t);
            int coef = (t % 2 == 0) ? 2 : -2;       /* 2*(-1)^t */
            sum += (long long)coef * S[i - sq];
        }
        S[i] = Narr[i] - sum;
    }

    long long s = S[N];
    long long ans = -(s / 5);           /* S[N] is a multiple of 5 */

    free(S);
    free(Narr);

    return (seq_t)ans;
}
