/*
 * A339968  Numbers k such that sigma(k) and k+2 are relatively prime, where
 *          sigma gives the sum of divisors of k.
 * https://oeis.org/A339968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k for which gcd(σ(k), k+2)=1. The
 *            function computes the sequence by incremental search, storing
 *            results in a static cache so repeated calls are fast
 * Range:     u8 n <= 111, u16 n <= 25061, u32 n <= 25064, u64 n <= 25064,
 *            u128 n <= 25064, big any n
 * Validated: standard, pass, 92 terms through n = 9443 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

/* Compute the sum of divisors σ(n) for an unsigned 64‑bit integer n. */
static seq_t sigma_uint64(uint64_t n)
{
    seq_t sum = 1;
    uint64_t rest = n;

    for (uint64_t p = 2; p * p <= rest; ++p) {
        if (rest % p == 0) {
            /* factor p^e */
            uint64_t power = 1;
            seq_t   contrib = 1;   /* 1 + p + p^2 + ... */
            while (rest % p == 0) {
                rest /= p;
                power *= p;
                contrib += (seq_t)power;
            }
            sum *= contrib;
        }
    }

    if (rest > 1) {          /* remaining prime factor */
        sum *= (seq_t)(1 + rest);
    }

    return sum;
}

/* External linkage required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n < 1 */

    static seq_t *cache   = NULL;   /* cached sequence values   */
    static size_t cached  = 0;      /* number of terms stored   */
    static size_t alloc   = 0;      /* current allocation size  */
    static idx_t  next    = 1;      /* next integer to test     */

    /* Grow the cache if it cannot hold the requested term. */
    if ((size_t)n > alloc) {
        size_t newalloc = alloc ? (alloc << 1) : 8;
        while (newalloc < (size_t)n) newalloc <<= 1;
        seq_t *tmp = mem_new(seq_t, newalloc);
        if (!tmp) return 0;                /* allocation failed */
        if (cache) {
            mem_copy(tmp, cache, cached * sizeof(seq_t));
            mem_free(cache);
        }
        cache = tmp;
        alloc = newalloc;
    }

    /* Generate terms until we have at least n of them. */
    while (cached < (size_t)n) {
        seq_t sigma = sigma_uint64((uint64_t)next);
        seq_t g     = gcd(sigma, (seq_t)(next + 2));
        if (g == 1) {
            cache[cached++] = (seq_t)next;
        }
        ++next;
    }

    /* Return the n‑th term (1‑based). */
    return cache[n - 1];
}
