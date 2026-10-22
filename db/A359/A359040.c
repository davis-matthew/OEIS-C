/*
 * A359040  Sum of the number of divisors of floor(n/(b*c)) with b,c > 0 and
 *          b*c <= n.
 * https://oeis.org/A359040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} d(k) * d(floor(n/k)) where d(m) is the number
 *            of positive divisors of m
 * Range:     u8 n <= 39, u16 n <= 4319, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Allocate an array to store the divisor counts up to n. */
    size_t sz = (size_t)n + 1;
    seq_t *divcnt = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!divcnt) return 0;

    /* Initialise to zero. */
    for (size_t i = 0; i < sz; ++i) divcnt[i] = 0;

    /* Compute divisor counts d(k) for all 1 <= k <= n. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            divcnt[j] += 1;
        }
    }

    /* Accumulate the sum Σ d(k) * d(floor(n/k)). */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t q = n / k;          /* floor(n/k) */
        unsigned long long prod = (unsigned long long)divcnt[k] *
                                  (unsigned long long)divcnt[q];
        sum += (seq_t)prod;
    }

    free(divcnt);
    return sum;
}
