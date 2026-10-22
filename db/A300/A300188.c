/*
 * A300188  a(n) = n! * [x^n] Product_{k>=1} 1/(1 + x^k)^(n/k).
 * https://oeis.org/A300188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * [x^n] exp(-n * Sum_{k≥1} s(k) * x^k / k) where s(k)
 *            = Σ_{d|k} (-1)^{d+1} Equivalent recurrence (for fixed N = n):
 *            let g[0] = N!, for m = 1 .. N: g[m] = -(N/m) * Σ_{k=1}^{m} s(k)
 *            * g[m-k] Then a(n) = g[N]
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 8, i64 n <= 14, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* compute s(k) = Σ_{d|k} (-1)^{d+1}  for 1 ≤ k ≤ N
   using a divisor sieve.  s is a signed 64 bit integer array. */
static void compute_s(int64_t *s, idx_t N)
{
    for (idx_t d = 1; d <= N; ++d) {
        int64_t sign = (d & 1ULL) ? 1LL : -1LL;
        for (idx_t m = d; m <= N; m += d) {
            s[m] += sign;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* a(0) = 1 */

    idx_t N = n;
    /* 1. Compute s(k) for 1..N */
    int64_t *s = (int64_t *)malloc((N + 1) * sizeof(int64_t));
    for (idx_t i = 0; i <= N; ++i) s[i] = 0;
    compute_s(s, N);

    /* 2. Compute factorial N! in seq_t */
    seq_t factN = 1;
    for (idx_t i = 1; i <= N; ++i) {
        factN = factN * (seq_t)i;
    }

    /* 3. Recurrence for g[0..N]  (g[0] = N!) */
    seq_t *g = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    g[0] = factN;
    for (idx_t m = 1; m <= N; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            sum += (seq_t)s[k] * g[m - k];
        }
        /* g[m] = -(N/m) * sum   , division exact */
        seq_t num = -(seq_t)n * sum;
        g[m] = num / (seq_t)m;
    }

    seq_t result = g[N];
    free(g);
    free(s);
    return result;
}
