/*
 * A354021  a(n) = Sum_{1 <= i < j < k < m <= n} (m*k*j*i)^2.
 * https://oeis.org/A354021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n-1)*(n-2)*(n-3)*(2*n + 1)*(2*n - 1)*(2*n -
 *            3)*(2*n - 5)*(5*n + 7)*(35*n^2 + 98*n + 72)/5443200
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 76, u128 n <= 3055,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    seq_t N = (seq_t)n;

    /* Denominator 5443200 = 2^7 * 3^5 * 5^2 * 7 */
    int primes[4] = {2, 3, 5, 7};
    int exps[4]   = {7, 5, 2, 1};

    /* Build the 11 numerator factors */
    seq_t fac[11];
    fac[0]  = N;
    fac[1]  = N + 1;
    fac[2]  = N - 1;
    fac[3]  = N - 2;
    fac[4]  = N - 3;
    fac[5]  = 2 * N + 1;
    fac[6]  = 2 * N - 1;
    fac[7]  = 2 * N - 3;
    fac[8]  = 2 * N - 5;
    fac[9]  = 5 * N + 7;
    fac[10] = 35 * N * N + 98 * N + 72;

    seq_t prod = 1;

    /* Multiply each factor, cancel denominator primes as they appear */
    for (int i = 0; i < 11; ++i) {
        seq_t f = fac[i];
        for (int j = 0; j < 4; ++j) {
            while (exps[j] > 0 && f % primes[j] == 0) {
                f /= (seq_t)primes[j];
                --exps[j];
            }
        }
        prod *= f;
    }

    /* All denominator primes have been cancelled */
    return prod;
}
