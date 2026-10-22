/*
 * A327277  Irregular triangle read by rows in which row n lists the first
 *          prime(n) primes.
 * https://oeis.org/A327277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Let S(k) = sum_{i=1}^k prime(i) (cumulative sum of the first k
 *            primes). For each n≥1 find the unique k such that S(k-1) < n ≤
 *            S(k). Then a(n) = prime( n - S(k-1) ). (The triangle has row k
 *            containing the first prime(k) primes, read row‑by‑row.)
 * Range:     u8 n <= 435, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* static data holding cumulative sums of primes */
    static idx_t *cumsum = NULL;      /* cumsum[0] = 0, cumsum[i] = Σ_{j=1}^i prime(j) */
    static size_t cap = 0;             /* allocated capacity */
    static size_t sz = 0;              /* number of valid entries (>=1) */

    /* Ensure cumsum[0] exists */
    if (cumsum == NULL) {
        cap = 16;
        cumsum = (idx_t *)malloc(cap * sizeof(idx_t));
        if (!cumsum) return 0;        /* unlikely, but defensive */
        cumsum[0] = 0;
        sz = 1;
    }

    /* Grow cumsum until the last entry is ≥ n */
    while (cumsum[sz - 1] < n) {
        /* compute next prime index = sz + 1 because sz is current count of entries */
        size_t next_idx = sz;          /* first missing index */
        uint32_t p = get_prime((idx_t)next_idx);

        /* make room if needed */
        if (sz >= cap) {
            size_t new_cap = cap * 2;
            idx_t *tmp = (idx_t *)realloc(cumsum, new_cap * sizeof(idx_t));
            if (!tmp) return 0;
            cumsum = tmp;
            cap = new_cap;
        }

        /* append new cumulative sum */
        cumsum[sz] = cumsum[sz - 1] + (idx_t)p;
        sz++;
    }

    /* Binary search for smallest k with cumsum[k] ≥ n */
    size_t lo = 1, hi = sz - 1;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (cumsum[mid] >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    size_t k = lo;

    /* t = n - S(k-1) */
    idx_t t = n - cumsum[k - 1];

    /* return prime(t), with t ≥ 1 */
    uint32_t result = get_prime(t);
    return (seq_t)result;
}
