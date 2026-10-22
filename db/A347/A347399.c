/*
 * A347399  a(n) = A347398(n^n).
 * https://oeis.org/A347399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n, k^k | n^n} k^k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/* static sieve of smallest prime factors (spf) */
#define MAX_SPF 200000
static uint32_t spf[MAX_SPF + 1] = {0};
static size_t spf_up_to = 0;

/* ensure spf is built up to max_n */
static void
ensure_spf(size_t max_n)
{
    if (max_n <= spf_up_to) return;
    if (max_n > MAX_SPF) max_n = MAX_SPF;           /* safety */
    for (size_t i = 2; i <= max_n; ++i) spf[i] = 0;
    for (size_t i = 2; i <= max_n; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= max_n) {
                for (size_t j = i * i; j <= max_n; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_up_to = max_n;
}

/* cache of k^k for k up to the largest n requested */
static seq_t *pow_cache = NULL;
static size_t pow_cache_cap = 0;   /* maximum k that has been cached (>=0) */

static void
ensure_pow_cache(size_t max_n)
{
    if (max_n <= pow_cache_cap) return;
    size_t new_cap = max_n + 1;          /* index 0 .. max_n */
    seq_t *new_cache = (seq_t *)realloc(pow_cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);             /* out of memory */
    for (size_t i = pow_cache_cap + 1; i <= max_n; ++i) {
        if (i == 0)
            new_cache[i] = (seq_t)0;     /* unused */
        else
            new_cache[i] = big_pow((seq_t)i, (seq_t)i);
    }
    /* Zero or keep old value for indices <= pow_cache_cap */
    pow_cache = new_cache;
    pow_cache_cap = max_n;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    size_t max_n = (size_t)n;
    ensure_spf(max_n);
    ensure_pow_cache(max_n);

    /* factor n */
    int primes_n[16];
    int exps_n[16];
    int cnt_n = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        uint32_t p = spf[(size_t)tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes_n[cnt_n] = (int)p;
        exps_n[cnt_n] = e;
        ++cnt_n;
    }

    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        /* factor k */
        int primes_k[16];
        int exps_k[16];
        int cnt_k = 0;
        idx_t t = k;
        while (t > 1) {
            uint32_t p = spf[(size_t)t];
            int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            primes_k[cnt_k] = (int)p;
            exps_k[cnt_k] = e;
            ++cnt_k;
        }

        bool ok = true;
        for (int i = 0; i < cnt_k && ok; ++i) {
            int j;
            for (j = 0; j < cnt_n; ++j)
                if (primes_n[j] == primes_k[i]) break;
            if (j == cnt_n) { ok = false; break; }

            idx_t lhs = (idx_t)exps_k[i] * k;
            idx_t rhs = (idx_t)exps_n[j] * n;
            if (lhs > rhs) { ok = false; break; }
        }

        if (ok) sum += pow_cache[k];
    }

    return sum;
}
