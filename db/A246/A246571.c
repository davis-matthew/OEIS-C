/*
 * A246571  G.f.: Sum_{n>=0} x^n / (1-x)^(4*n+3) * [Sum_{k=0..2*n+1}
 *          C(2*n+1,k)^2 * x^k]^2.
 * https://oeis.org/A246571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Sum_{j=0..k} C(2*n - k - j + 1, k)^2 * C(k,
 *            j)^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes binomial coefficient C(N,K) for non‑negative 64‑bit N and K (K <= N) */
static seq_t binom_u64(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K == 0 || K == N) return (seq_t)1;
    if (K > N - K) K = N - K;          /* use the smaller side */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= K; ++i) {
        res = res * ((seq_t)(N - K + i)) / (seq_t)i;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t total = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Binomial C(2*n - k + 1, k) for j = 0 */
        seq_t binom_N = binom_u64(2*n - k + 1, k);
        seq_t curN = (seq_t)(2*n - k + 1);

        seq_t binom_kj = (seq_t)1;   /* C(k,0) */

        for (idx_t j = 0; j <= k; ++j)
        {
            /* term = binom_N^2 * binom_kj^2 */
            seq_t t1 = binom_N * binom_N;
            seq_t t2 = binom_kj * binom_kj;
            total += t1 * t2;

            if (j < k)
            {
                /* Update binom_N for next j: C(curN-1, k) = binom_N * (curN - k) / curN */
                binom_N = binom_N * (curN - (seq_t)k) / curN;
                curN -= 1;

                /* Update binom_kj for next j: C(k, j+1) = binom_kj * (k - j) / (j+1) */
                binom_kj = binom_kj * ((seq_t)(k - j)) / (seq_t)(j + 1);
            }
        }
    }

    return total;
}
