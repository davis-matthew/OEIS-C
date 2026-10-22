/*
 * A349994  a(n) is the difference between the n-th prime and the next smaller
 *          odd squarefree semiprime, divided by 2.
 * https://oeis.org/A349994
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_n - s)/2 where p_n is the n-th prime and s is the
 *            largest odd squarefree semiprime less than p_n
 * Range:     u8 n <= 54, u16 n <= 7064, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    /* static storage for primes */
    static bool initialized = false;
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;
    const idx_t OFFSET = 7;          /* offset as per OEIS */

    /* Initialize once: generate all primes up to a safe upper bound. */
    if (!initialized) {
        const idx_t UPPER_LIMIT = 200000;          /* > 10th‑thousand‑th prime */
        build_primes_upto(UPPER_LIMIT, &primes, &prime_count);
        initialized = true;
    }

    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */

    if ((idx_t)prime_count < n) return (seq_t)0;  /* out of range */

    uint64_t p = primes[n - 1];            /* n-th prime (1‑based) */

    /* Find the first index of a prime >= p */
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* `lo` is the index of the first prime >= p */
    size_t start = lo == 0 ? 0 : lo - 1;   /* last prime < p */

    seq_t max_prod = 0;

    /* search over all odd primes < p, from largest to smallest */
    for (size_t idx = start; idx > 0; --idx) {   /* idx >=1 guarantees odd prime */
        uint64_t r = primes[idx];
        if (r <= 2) continue;   /* skip non‑odd prime (2) */

        /* Find the largest odd prime q < r with r*q < p */
        size_t j = idx - 1;
        while (j > 0 && (uint64_t)primes[j] * r >= p) {
            --j;
        }
        if (j == 0) continue;              /* no suitable q */
        if (primes[j] == 2) continue;       /* ensure q is odd */

        seq_t prod = (seq_t)primes[j] * (seq_t)r;
        if (prod > max_prod)
            max_prod = prod;
        if (max_prod >= (seq_t)(p - 1))
            break;                         /* cannot find larger */
    }

    /* Compute the difference divided by 2 */
    seq_t diff = ((seq_t)p - max_prod) / 2;
    return diff;
}
