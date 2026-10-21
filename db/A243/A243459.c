/*
 * A243459  Primes p such that (p reversed)-5 is also a prime.
 * https://oeis.org/A243459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime p for which reverse(p)-5 is also prime
 *            (offset 1)
 * Range:     u8 n <= 7, u16 n <= 527, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Helper: miller-rabin for 64‑bit integers ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small prime test */
    static const uint64_t small[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    /* write n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }
    /* Deterministic bases for 64‑bit */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = d;
        uint64_t base = a;
        while (p) {
            if (p & 1ULL) x = (x * base) % n;
            base = (uint64_t)((__uint128_t)base * base % n);
            p >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ---------- Helper: reverse decimal digits ---------- */
static uint64_t rev_uint64(uint64_t n)
{
    uint64_t r = 0;
    while (n) {
        r = r * 10 + n % 10;
        n /= 10;
    }
    return r;
}

/* ---------- Sequence cache ---------- */
static seq_t  *s_cache  = NULL;
static size_t  s_len    = 0;
static size_t  s_cap    = 0;
static uint64_t s_last  = 1;   /* last prime examined */

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1 */
    size_t nn = (size_t)n;
    if (s_len < nn) {
        if (s_cache == NULL) {
            s_cap = 64;
            s_cache = (seq_t*)malloc(s_cap * sizeof(seq_t));
        }
        while (s_len < nn) {
            /* find next prime */
            uint64_t cand = s_last + 1;
            while (!is_prime_uint64(cand)) ++cand;
            s_last = cand;
            /* check reverse(p)-5 */
            uint64_t revp = rev_uint64(cand);
            if (revp > 5) {
                uint64_t diff = revp - 5;
                if (is_prime_uint64(diff)) {
                    if (s_len == s_cap) {
                        s_cap *= 2;
                        s_cache = (seq_t*)realloc(s_cache, s_cap * sizeof(seq_t));
                    }
                    s_cache[s_len++] = (seq_t)cand;
                }
            }
        }
    }
    return s_cache[nn - 1];
}
