/*
 * A282845  Number of ways to write n as an ordered sum of 6 prime power
 *          palindromes (A084092).
 * https://oeis.org/A282845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   number of ways to write n as an ordered sum of 6 prime‑power
 *            palindromes (A084092)
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* small primes needed for prime‑power test (≤ 100) */
static const uint32_t primes_small[] =
{ 2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
  31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
  73, 79, 83, 89, 97 };
static const size_t prime_cnt = sizeof(primes_small) / sizeof(primes_small[0]);

/* check if x is a prime power, counting 1 as a prime power. */
static bool is_prime_power(uint64_t x)
{
    if (x == 1) return true;
    if (x < 2)  return false;
    uint64_t tmp = x;
    int distinct = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes_small[i] * primes_small[i] <= tmp; ++i) {
        uint32_t p = primes_small[i];
        if (tmp % p == 0) {
            ++distinct;
            if (distinct > 1) return false;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {
        ++distinct;
        if (distinct > 1) return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    size_t maxN = (size_t)n;
    /* list of prime‑power palindromes ≤ n (including 1) */
    static size_t spp[1100];
    size_t spc = 0;

    /* 1‑digit (1‑9) */
    for (uint32_t d = 1; d <= 9; ++d) {
        uint64_t val = d;
        if (val > maxN) break;
        if (is_prime_power(val)) spp[spc++] = val;
    }
    /* 2‑digit (aa) */
    for (uint32_t a = 1; a <= 9; ++a) {
        uint64_t val = a * 10 + a;
        if (val > maxN) break;
        if (is_prime_power(val)) spp[spc++] = val;
    }
    /* 3‑digit (aba) */
    for (uint32_t a = 1; a <= 9; ++a) {
        for (uint32_t b = 0; b <= 9; ++b) {
            uint64_t val = a * 100 + b * 10 + a;
            if (val > maxN) break;
            if (is_prime_power(val)) spp[spc++] = val;
        }
    }
    /* 4‑digit (abba) */
    for (uint32_t a = 1; a <= 9; ++a) {
        for (uint32_t b = 0; b <= 9; ++b) {
            uint64_t val = a * 1000 + b * 100 + b * 10 + a;
            if (val > maxN) break;
            if (is_prime_power(val)) spp[spc++] = val;
        }
    }
    /* 5‑digit (abcba) – only if 5‑digit ≤ n (nominally >10000 for most tests) */
    for (uint32_t a = 1; a <= 9; ++a) {
        for (uint32_t b = 0; b <= 9; ++b) {
            for (uint32_t c = 0; c <= 9; ++c) {
                uint64_t val = a * 10000 + b * 1000 + c * 100 + b * 10 + a;
                if (val > maxN) break;
                if (is_prime_power(val)) spp[spc++] = val;
            }
        }
    }

    /* dynamic programming for convolution of 6 copies */
    size_t M = maxN + 1;
    seq_t *ways = (seq_t *)calloc(M, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(M, sizeof(seq_t));

    /* k = 1 */
    for (size_t i = 0; i < spc; ++i) {
        size_t p = spp[i];
        ways[p] = 1;
    }

    /* k = 2 .. 6 */
    for (int k = 2; k <= 6; ++k) {
        memset(next, 0, M * sizeof(seq_t));
        for (size_t s = 0; s < M; ++s) {
            seq_t val = ways[s];
            if (!val) continue;
            for (size_t i = 0; i < spc; ++i) {
                size_t p = spp[i];
                size_t ns = s + p;
                if (ns > maxN) break;          /* spp is sorted */
                next[ns] += val;
            }
        }
        /* swap arrays for next iteration */
        seq_t *tmp = ways;
        ways = next;
        next = tmp;
    }

    seq_t result = ways[maxN];
    free(ways);
    free(next);
    return result;
}
