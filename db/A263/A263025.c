/*
 * A263025  n is the a(n)-th positive integer having its sum of divisors.
 * https://oeis.org/A263025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 1278, u16 n <= 25000, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 103 terms through n = 7858 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by sieving sum‑of‑divisors (σ) up to n and counting how
   many numbers ≤ n have the same σ value.  The result is the rank of
   n among the numbers that share its σ. */
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Fast return for the single known term at n = 1 */
    if (n == 1) return 1;

    /* Convert to size_t for array bounds; n is small (≤ 10000) */
    size_t N = (size_t)n;

    /* Allocate and compute σ[1..N] using a divisor sieve */
    seq_t *sigma = mem_new(seq_t, N + 1);
    if (!sigma) return 0;   /* allocation failure */

    for (idx_t i = 1; i <= N; ++i) {
        seq_t si = (seq_t)i;
        for (idx_t j = i; j <= N; j += i) {
            sigma[j] += si;
        }
    }

    /* Find the maximum σ to size the count array */
    seq_t max_sigma = 0;
    for (idx_t i = 1; i <= N; ++i) {
        if (sigma[i] > max_sigma) max_sigma = sigma[i];
    }
    size_t max_sigma_idx = (size_t)max_sigma;   /* safe: σ is small */

    /* Count how many times each σ has appeared so far */
    seq_t *cnt = mem_new(seq_t, max_sigma_idx + 1);
    if (!cnt) {
        mem_free(sigma);
        return 0;
    }

    seq_t result = 0;
    for (idx_t i = 1; i <= N; ++i) {
        size_t idx = (size_t)sigma[i];
        ++cnt[idx];
        if (i == N) result = cnt[idx];
    }

    mem_free(sigma);
    mem_free(cnt);
    return result;
}
