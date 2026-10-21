/*
 * A280458  Partial products of A023896.
 * https://oeis.org/A280458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prod_{i=1..n} (i*phi(i)/2), where phi is Euler's totient
 *            * (phi(1)=1). The first term of the sequence is 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *phi_cache = NULL;   /* stores φ(i) for i <= current cacheSize */
static seq_t   *prod_cache = NULL;   /* stores a(i) for i <= current cacheSize */
static size_t   cacheSize   = 0;     /* highest index for which values are cached */

/* Compute Euler's totient function for n (n >= 1).  The algorithm runs in
 * O(sqrt(n)) time, which is adequate for n up to at least 10000. */
static uint64_t phi_uint64(uint64_t v)
{
    if (v == 0) return 0;
    uint64_t result = v;
    uint64_t n = v;

    /* handle factor 2 */
    if ((n & 1) == 0) {
        while ((n & 1) == 0) n >>= 1;
        result -= result >> 1;
    }

    /* handle odd prime factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }

    /* if n > 1, then n itself is a prime factor */
    if (n > 1)
        result -= result / n;

    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but guard against 0 */

    /* Extend the cache if necessary */
    if (cacheSize < (size_t)n) {
        size_t newSize = (size_t)n;               /* inclusive size needed */
        /* Allocate or grow the phi and product caches. */
        if (phi_cache == NULL) {
            phi_cache  = (uint64_t *)malloc(sizeof(uint64_t) * (newSize + 1));
            prod_cache = (seq_t *)   malloc(sizeof(seq_t)   * (newSize + 1));
        } else {
            phi_cache  = (uint64_t *)realloc(phi_cache,  sizeof(uint64_t) * (newSize + 1));
            prod_cache = (seq_t *)   realloc(prod_cache, sizeof(seq_t)   * (newSize + 1));
        }

        /* Compute values for indices cacheSize+1 .. newSize. */
        for (size_t i = cacheSize + 1; i <= newSize; ++i) {
            if (i == 1) {
                phi_cache[i] = 1;
                prod_cache[i] = (seq_t)1;
            } else {
                phi_cache[i] = phi_uint64((uint64_t)i);
                uint64_t temp = ((uint64_t)i * phi_cache[i]) / 2;
                seq_t mult = (seq_t)temp;
                prod_cache[i] = prod_cache[i - 1] * mult;
            }
        }
        cacheSize = newSize;
    }

    return prod_cache[n];
}
