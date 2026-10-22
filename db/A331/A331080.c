/*
 * A331080  a(n) = Sum_{i=1..n} d_3(i)*d_3(i+1), where d_3(n) = A007425(n).
 * https://oeis.org/A331080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} d3(i) * d3(i+1), where d3(k) = Sum_{d|k}
 *            tau(d) and tau(k) is the number of divisors of k
 * Range:     u8 n <= 9, u16 n <= 268, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* we need d3 for values up to n+1 */
    idx_t M = n + 1;
    /* allocate arrays for tau and d3 */
    seq_t *tau = (seq_t*)calloc((size_t)(M + 1), sizeof(seq_t));
    seq_t *d3  = (seq_t*)calloc((size_t)(M + 1), sizeof(seq_t));
    if (!tau || !d3) { free(tau); free(d3); return 0; }

    /* compute tau(k) for all k <= M */
    for (idx_t i = 1; i <= M; ++i) {
        for (idx_t j = i; j <= M; j += i) {
            tau[j] += (seq_t)1;
        }
    }

    /* compute d3(k) = sum_{d|k} tau(d) */
    for (idx_t i = 1; i <= M; ++i) {
        seq_t val = tau[i];
        for (idx_t j = i; j <= M; j += i) {
            d3[j] += val;
        }
    }

    /* accumulate the sum */
    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        sum += d3[i] * d3[i + 1];
    }

    free(tau);
    free(d3);
    return sum;
}
