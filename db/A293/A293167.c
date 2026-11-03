/*
 * A293167  a(n) = Sum_{k = 1..n} d(d(d(k))), where d(k) is the number of
 *          divisors of k (A000005).
 * https://oeis.org/A293167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k = 1..n} d(d(d(k))) where d is the divisor count
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9450 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute the number of divisors of a small integer n (n is assumed small, e.g. n <= 2000). */
static inline idx_t divisor_count_small(idx_t n)
{
    idx_t cnt = 0;
    for (idx_t i = 1; i*i <= n; ++i) {
        if (n % i == 0) {
            cnt += (i*i == n) ? 1 : 2;
        }
    }
    return cnt;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Allocate an array to hold divisor counts for 1..n. */
    idx_t *d = (idx_t *)malloc((size_t)(n + 1) * sizeof(idx_t));
    if (!d) return 0;  /* Allocation failure: return 0 as a safe fallback. */

    /* Initialise with zeros. */
    for (idx_t i = 1; i <= n; ++i) d[i] = 0;

    /* Sieve‑like computation of divisor counts. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            d[j] += 1;
        }
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t dk  = d[k];
        idx_t ddk = divisor_count_small(dk);
        idx_t dddk = divisor_count_small(ddk);
        sum += (seq_t)dddk;
    }

    free(d);
    return sum;
}
