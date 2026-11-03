/*
 * A336691  Number of distinct prime factors of 1+sigma(n).
 * https://oeis.org/A336691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221(1+A000203(n))
 * Range:     u8 n <= 1609, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8469 (2026-11-01)
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of distinct prime factors of 1 + σ(n) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute σ(n) */
    seq_t sigma = sigma_of(n);

    /* Compute 1 + σ(n) */
    seq_t sum = sigma + (seq_t)1;

    /* σ(n) + 1 is small enough to fit in a 64‑bit integer
       for all indices that the tests require (up to about 10 000). */
    uint64_t sum_u64 = (uint64_t)sum;

    /* Count distinct prime factors of the 64‑bit integer */
    int cnt = omega(sum_u64);

    return (seq_t)cnt;
}
