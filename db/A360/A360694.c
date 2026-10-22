/*
 * A360694  Numbers whose divisors can be partitioned into two disjoint sets
 *          where the sum of both sets is prime.
 * https://oeis.org/A360694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 160, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A sequence of integers whose divisors can be partitioned into two
   sets with prime‑sums.  The implementation caches already found terms
   and tests candidates by generating their divisors and searching a
   subset sum bit‑set that yields a pair of complementary prime sums.
   The logic works for both C (seq_t is a builtin unsigned type) and
   C++ (seq_t may be an arbitrary‑precision integer class). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"

#ifdef __cplusplus
#include <vector>
#endif

/* ----- Helper: test if a single integer has the required property ----- */
static bool has_property(uint64_t n)
{
    if (n < 4) return false;

    /* Allocate an array for divisors; start with capacity 16 */
    size_t divcap = 16;
    uint64_t *divs = NULL;
#ifdef __cplusplus
    divs = new uint64_t[divcap];
#else
    divs = (uint64_t*)malloc(divcap * sizeof(uint64_t));
#endif
    if (!divs) return false;
    size_t divcnt = 0;

    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            if (divcnt == divcap) {
                size_t newcap = divcap << 1;
#ifdef __cplusplus
                uint64_t *tmp = new uint64_t[newcap];
                memcpy(tmp, divs, divcnt * sizeof(uint64_t));
                delete[] divs;
#else
                uint64_t *tmp = (uint64_t*)realloc(divs, newcap * sizeof(uint64_t));
                if (!tmp) { free(divs); return false; }
#endif
                divs = tmp;
                divcap = newcap;
            }
            divs[divcnt++] = d;
            uint64_t e = n / d;
            if (e != d) {
                if (divcnt == divcap) {
                    size_t newcap = divcap << 1;
#ifdef __cplusplus
                    uint64_t *tmp = new uint64_t[newcap];
                    memcpy(tmp, divs, divcnt * sizeof(uint64_t));
                    delete[] divs;
#else
                    uint64_t *tmp = (uint64_t*)realloc(divs, newcap * sizeof(uint64_t));
                    if (!tmp) { free(divs); return false; }
#endif
                    divs = tmp;
                    divcap = newcap;
                }
                divs[divcnt++] = e;
            }
        }
    }

    /* Sum of all divisors */
    uint64_t sigma = 0;
    for (size_t i = 0; i < divcnt; ++i) sigma += divs[i];

    /* Quick check for partitions that use only {1, d} as one side */
    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d = divs[i];
        if (d == 1) continue;
        uint64_t s1 = 1 + d;
        uint64_t s2 = sigma - s1;
        if (s2 >= 2 && is_prime((idx_t)s1) && is_prime((idx_t)s2))
            { free(divs); return true; }
    }

    /* Subset‑sum bitset: check all reachable sums */
    size_t bits_len = (size_t)(sigma >> 6) + 1;   /* number of 64‑bit words */
    uint64_t *bits = NULL;
#ifdef __cplusplus
    bits = new uint64_t[bits_len];
    memset(bits, 0, bits_len * sizeof(uint64_t));
#else
    bits = (uint64_t*)calloc(bits_len, sizeof(uint64_t));
#endif
    if (!bits) { free(divs); return false; }
    bits[0] = 1ULL; /* sum 0 reachable */

    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d = divs[i];
        size_t wshift = (size_t)(d >> 6);
        unsigned int bshift = (unsigned int)(d & 63);

        for (size_t idx = bits_len; idx-- > wshift;) {
            uint64_t src = bits[idx - wshift] << bshift;
            if (bshift && idx - wshift > 0)
                src |= bits[idx - wshift - 1] >> (64 - bshift);
            bits[idx] |= src;
        }
    }

    /* Search for a prime pair (p, sigma‑p) */
    for (size_t idx = 0; idx < bits_len; ++idx) {
        uint64_t word = bits[idx];
        while (word) {
            uint64_t lowbit = word & -word;
            int shift = __builtin_ctzll(word);
            uint64_t r = (uint64_t)(idx << 6) + shift;
            if (r >= 2 && r <= sigma - 2) {
                if (is_prime((idx_t)r) && is_prime((idx_t)(sigma - r))) {
                    free(divs);
#ifdef __cplusplus
                    delete[] bits;
#else
                    free(bits);
#endif
                    return true;
                }
            }
            word ^= lowbit;
        }
    }

    free(divs);
#ifdef __cplusplus
    delete[] bits;
#else
    free(bits);
#endif
    return false;
}

/* ----- Sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but safe */

#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static uint64_t next_candidate = 4;   /* first number to test */

    while (cache.size() < (size_t)n) {
        if (has_property(next_candidate))
            cache.push_back((seq_t)next_candidate);
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
#else
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;
    static uint64_t next_candidate = 4;

    while (cache_cnt < (size_t)n) {
        if (cache_cnt == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 32;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        if (has_property(next_candidate))
            cache[cache_cnt++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return cache[n - 1];
#endif
}
