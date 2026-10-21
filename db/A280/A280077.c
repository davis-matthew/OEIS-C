/*
 * A280077  Partial sums of A007429 (Sum_{d|n} sigma(d)).
 * https://oeis.org/A280077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} sigma(k) * floor(n/k)
 * Range:     u8 n <= 14, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
/* In C++ allocate with new[] and free with delete[] */
static seq_t *seq_alloc(idx_t size) { return new seq_t[size]; }
static void seq_free(seq_t *p) { delete[] p; }
#else
/* In C use malloc/free */
static seq_t *seq_alloc(idx_t size) { return (seq_t *)malloc((size_t)size * sizeof(seq_t)); }
static void seq_free(seq_t *p) { free(p); }
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety guard – offset is 1 */

    /* Allocate array for sigma[1..n] (index 0 unused) */
    seq_t *sigma = seq_alloc(n + 1);
    /* Initialize sigma array to zero */
    for (idx_t i = 0; i <= n; ++i) sigma[i] = (seq_t)0;

    /* Compute sigma(k) by a linear sieve: sum of divisors */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            sigma[j] = sigma[j] + (seq_t)i;
        }
    }

    /* Compute a(n) = Sum_{k=1..n} sigma(k) * floor(n/k) */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t q = n / k;               /* floor(n/k) */
        sum = sum + sigma[k] * (seq_t)q;
    }

    seq_free(sigma);
    return sum;
}
