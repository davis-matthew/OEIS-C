/*
 * A392500  Numbers k > 1 whose largest proper divisor cannot be written as a
 *          sum of their other nontrivial divisors.
 * https://oeis.org/A392500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth composite number whose largest proper divisor
 *            cannot be expressed as a sum of its other nontrivial divisors.
 *            The implementation uses a sieve to obtain smallest prime factors
 *            and a subset-sum dynamic programming test for each composite
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   shift_or_inplace
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/shift_or_inplace.h"

#ifndef LIMIT
#define LIMIT 200000 /* enough for n up to ~10000 */
#endif

/* Smallest prime factor sieve */
static uint64_t *spf = NULL;
static size_t spfSize = 0;

static void init_spf(void)
{
    if (spf) return; /* already initialized */

    size_t limit = LIMIT;
    spf = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i <= limit / i) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spfSize = limit + 1;
}

/* Bitset used for subset sum – static to avoid allocations on each call */
#define MAX_TARGET (LIMIT >> 1)         /* limit / 2 */
#define MAX_WORDS ((MAX_TARGET + 63) / 64 + 2) /* +2 for safety */
static uint64_t bits[MAX_WORDS];

/* Test if k is a noncomposable number. */
static bool is_noncomposable(uint64_t k)
{
    uint64_t spf_k = spf[k];
    uint64_t target = k / spf_k;     /* largest proper divisor */

    /* Generate all divisors of k */
    uint64_t divs[256];
    size_t dcnt = 1;
    divs[0] = 1;

    uint64_t tmp = k;
    while (tmp > 1) {
        uint64_t p = spf[tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        size_t old = dcnt;
        uint64_t mult = 1;
        for (int i = 1; i <= e; ++i) {
            mult *= p;
            for (size_t j = 0; j < old; ++j)
                divs[dcnt++] = divs[j] * mult;
        }
    }

    /* Prepare subset-sum DP */
    size_t wordCount = (target >> 6) + 1;
    for (size_t i = 0; i < wordCount; ++i) bits[i] = 0;
    bits[0] = 1;
    size_t targetWord = target >> 6;
    uint64_t targetMask = 1ULL << (target & 63);

    /* Process each divisor except 1, k, and target itself */
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        if (d == 1 || d == k || d == target) continue;

        shift_or_inplace(bits, wordCount, (size_t)d);

        if (bits[targetWord] & targetMask)
            return false; /* composable */
    }
    return true; /* noncomposable */
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but guard */

    init_spf();

    static seq_t *cache = NULL;
    static size_t cacheSize = 0;
    static size_t cacheCap = 0;
    static uint64_t candidate = 4; /* next number to test */

    while (cacheSize < (size_t)n) {
        if (candidate < spfSize && spf[candidate] == candidate) {
            /* prime – skip */
        } else {
            if (is_noncomposable(candidate)) {
                if (cacheCap == 0) {
                    cacheCap = 16;
                    cache = (seq_t *)malloc(cacheCap * sizeof(seq_t));
                } else if (cacheSize == cacheCap) {
                    cacheCap <<= 1;
                    cache = (seq_t *)realloc(cache, cacheCap * sizeof(seq_t));
                }
                cache[cacheSize++] = (seq_t)candidate;
            }
        }
        ++candidate;
    }
    return cache[(size_t)n - 1];
}
