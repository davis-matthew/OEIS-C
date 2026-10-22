/*
 * A396545  Numbers that are equal to the sum of their coreful divisors that
 *          are not exponential divisors.
 * https://oeis.org/A396545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is a number such that the sum of its coreful divisors that
 *            are not exponential divisors equals the number itself
 * Range:     u8 none, u16 n <= 51, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Build smallest prime factor array up to LIMIT */
#define LIMIT 20000000u

static uint32_t *spf = NULL;

static void build_spf(void)
{
    size_t i, j;
    spf = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!spf) exit(1);
    for (i = 0; i <= LIMIT; ++i) spf[i] = 0;
    spf[0] = 0; spf[1] = 1;
    for (i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= LIMIT) {
                for (j = (uint64_t)i * i; j <= LIMIT; j += i) {
                    if (spf[(uint32_t)j] == 0) spf[(uint32_t)j] = (uint32_t)i;
                }
            }
        }
    }
}

/* Cache of computed sequence terms */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t next_test = 576;          /* first possible term */

/* Ensure at least 'need' terms are cached */
static void ensure_cache(idx_t need)
{
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    /* If spf not built yet */
    if (!spf) build_spf();

    while (cache_len < (size_t)need && next_test <= LIMIT) {
        uint32_t n = (uint32_t)next_test;
        seq_t prodX = (seq_t)1;
        seq_t prodY = (seq_t)1;
        seq_t g     = (seq_t)1;
        bool has_power = false;
        uint32_t t = n;

        while (t > 1) {
            uint32_t p = spf[t];
            idx_t exp = 0;
            while (t % p == 0) {
                t /= p;
                exp++;
            }
            if (exp >= 2) {
                has_power = true;
                seq_t p_seq = (seq_t)p;
                seq_t sumA = 0;
                seq_t sumB = 0;
                seq_t powP = 1;
                for (idx_t e = 1; e <= exp; ++e) {
                    powP = powP * p_seq;
                    sumA += powP;
                    if (exp % e == 0) {
                        sumB += powP;
                    }
                }
                prodX = prodX * sumA;
                prodY = prodY * sumB;
                g     = g * powP;   /* powP is now p^exp */
            }
        }

        if (has_power && prodX > prodY) {
            seq_t diff = prodX - prodY;
            if (diff == g) {
                if (cache_len >= cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!cache) exit(1);
                }
                cache[cache_len++] = (seq_t)n;
            }
        }
        next_test++;
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n-1];
}
