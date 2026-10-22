/*
 * A350368  Triangular numbers that are the sum of two distinct nonzero
 *          triangular numbers in exactly two ways.
 * https://oeis.org/A350368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth triangular number T_k such that T_k = T_i + T_j for
 *            exactly two distinct positive indices i < j. T_m = m(m+1)/2
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---- Prime sieve (global, built on first use) ---- */
static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_list != NULL) return;      /* already done */
    const uint64_t LIMIT = 1000000;         /* > max sqrt needed */
    bool *is_comp = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    memset(is_comp, 0, (LIMIT + 1) * sizeof(bool));

    for (uint64_t p = 2; p * p <= LIMIT; ++p)
        if (!is_comp[p])
            for (uint64_t m = p * p; m <= LIMIT; m += p)
                is_comp[m] = true;

    /* Count primes first */
    prime_cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++prime_cnt;

    prime_list = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    for (uint64_t i = 2, idx = 0; i <= LIMIT; ++i)
        if (!is_comp[i]) prime_list[idx++] = (uint32_t)i;

    free(is_comp);
}

/* ---- Integer square root (floor) for 64‑bit values ---- */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;   /* hi > sqrt(max n) */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) { lo = 1; continue; }
        uint64_t q = n / mid;
        if (mid <= q) {
            if (mid + 1 > q) return mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return lo - 1;
}

/* ---- Sequence cache (global) ---- */
static idx_t cache_size = 0;
static seq_t *cache = NULL;
static idx_t built_terms = 0;      /* number of terms stored in cache */
static idx_t next_k = 0;           /* next triangular index to examine */

/* ---- Main function implementation ---- */
seq_t a(idx_t n)
{
    /* initialise primes if not already */
    init_primes();

    /* enlarge cache if needed */
    if (n > cache_size) {
        cache = (seq_t*)realloc(cache, (size_t)n * sizeof(seq_t));
        cache_size = n;
    }

    /* Generate terms until we have at least n */
    while (built_terms < n) {
        /* Search next k that satisfies the condition */
        for (;;) {
            ++next_k;                     /* next triangular index */
            uint64_t k = (uint64_t)next_k;

            /* N = (2k+1)^2 + 1 */
            uint64_t Z = 2 * k + 1;
            uint64_t N = Z * Z + 1;

            /* Factor N, compute product R of (e+1) over primes ≡1 (mod 4) */
            uint64_t tmp = N;
            uint64_t R = 1;
            bool bad = false;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint32_t p = prime_list[i];
                if ((uint64_t)p * p > tmp) break;
                if (tmp % p == 0) {
                    uint32_t exp = 0;
                    while (tmp % p == 0) { tmp /= p; ++exp; }
                    if ((p & 3) == 3) {          /* prime ≡3 (mod 4) */
                        if (exp & 1) { bad = true; break; }
                    } else if ((p & 3) == 1) {   /* prime ≡1 (mod 4) */
                        R *= (uint64_t)(exp + 1);
                    }
                }
            }
            if (bad) continue;              /* no representations */

            if (tmp > 1) {                  /* remaining prime factor */
                uint32_t p = (uint32_t)tmp;
                if ((p & 3) == 3) {         /* odd exponent of ≡3 prime */
                    bad = true;
                } else {                    /* p ≡1 (mod 4) with exp = 1 */
                    R *= 2;
                }
            }
            if (bad) continue;

            /* Check whether N/2 is a perfect square (equal a,b case) */
            uint64_t half = N >> 1;                /* N is even */
            uint64_t s = isqrt_u64(half);
            bool equal_exists = (s * s == half);

            /* Condition: R == 6 (no equal) or R == 7 (equal exists) */
            if ((equal_exists && R == 7) || (!equal_exists && R == 6)) {
                /* Record the triangular number */
                seq_t term = (seq_t)k * ((seq_t)k + 1) / 2;
                cache[built_terms++] = term;
                break;      /* move on to next required term */
            }
        }
    }
    return cache[n - 1];
}
