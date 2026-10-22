/*
 * A392288  Nonsquarefree numbers whose canonical prime factorization has at
 *          least two prime power factors with equal exponents.
 * https://oeis.org/A392288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392288: Nonsquarefree numbers whose canonical prime factorization
 * has at least two prime power factors with equal exponents.
 * The function computes the sequence by brute‑force search, testing each integer
 * for the property and caching the found terms.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static cache for already computed terms. */
static seq_t *cache = NULL;      /* array of terms */
static size_t cacheCap = 0;      /* allocated capacity */
static size_t cacheSize = 0;     /* number of terms stored */

/* Last integer that has been examined in the search. */
static uint64_t lastChecked = 1;

/* Ensure the cache has enough capacity for `needed` elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cacheCap)
        return;
    size_t newCap = cacheCap ? cacheCap * 2 : 64;
    while (newCap < needed)
        newCap *= 2;
    seq_t *newCache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
    cache = newCache;
    cacheCap = newCap;
}

/* Test whether x satisfies the sequence property.
 * x is assumed to be a positive integer larger than 1. */
static bool satisfies(uint64_t x)
{
    bool hasDup = false;          /* at least two equal exponents */
    bool nonSqf = false;          /* at least one exponent >= 2 */

    /* Small array to count exponents; largest exponent for 64‑bit numbers
     * is bounded by log2(x), which is < 64. */
    uint8_t expCount[65] = {0};

    uint64_t n = x;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p != 0)
            continue;
        uint8_t e = 0;
        while (n % p == 0)
        {
            n /= p;
            ++e;
        }
        if (e > 1)
            nonSqf = true;
        expCount[e] += 1;
        if (expCount[e] >= 2)
            hasDup = true;
    }
    if (n > 1)          /* remaining prime factor */
    {
        uint8_t e = 1;   /* exponent 1 for the remaining prime */
        expCount[e] += 1;
        if (expCount[e] >= 2)
            hasDup = true;
        /* e == 1, so nonSqf unchanged */
    }
    return nonSqf && hasDup;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n >= 1 according to the OEIS offset. */
    if (n <= 0)
        return 0; /* Undefined for n <= 0, but return 0 to avoid UB. */

    if ((size_t)n > cacheSize)
    {
        size_t target = (size_t)n;
        for (;;)
        {
            ++lastChecked;
            if (satisfies(lastChecked))
            {
                ensure_capacity(cacheSize + 1);
                cache[cacheSize++] = (seq_t)lastChecked;
                if ((size_t)n == cacheSize)
                    break;
            }
            /* In practice this loop will only iterate until we reach
             * the desired nth term. */
        }
    }
    return cache[(size_t)n - 1];
}
