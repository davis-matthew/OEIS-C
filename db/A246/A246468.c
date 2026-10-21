/*
 * A246468  Given a number of k digits x = d_(k)*10^(k-1) + d_(k-1)*10^(k-2) +
 *          … + d_(2)*10 + d_(1), consider y =
 *          p_(1)^d_(1)*p_(2)^d_(2)*…*p_(k)^d_(k), where p_(i) is the i-th
 *          prime. Sequence lists the numbers x such that y / x is an integer.
 * https://oeis.org/A246468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers x such that ∏_{i=1}^{k} p_i^{d_i} divides x, where d_i
 *            are decimal digits of x and p_i is the i‑th prime. Brute‑force
 *            search
 * Range:     u8 n <= 17, u16 n <= 142, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* first 31 primes */
static const uint64_t SMALL_PRIMES[] = {
    2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,
    31ULL,37ULL,41ULL,43ULL,47ULL,53ULL,59ULL,61ULL,67ULL,71ULL,
    73ULL,79ULL,83ULL,89ULL,97ULL,101ULL,103ULL,107ULL,109ULL,113ULL,127ULL
};

/* multiply modulo a 64‑bit number using 128‑bit arithmetic */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    if (mod == 1) return 0;
    return (unsigned __int128)a * b % mod;
}

/* modular exponentiation (exponent ≤ 9) */
static inline uint64_t powmod_uint64(uint64_t base, int exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = mul_mod_uint64(res, base, mod);
        base = mul_mod_uint64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* check the divisibility property for a 64‑bit integer */
static inline bool good_uint64(uint64_t x)
{
    uint64_t orig = x;
    uint64_t prod = 1 % orig;
    uint64_t t = x;
    int digit[32];
    int len = 0;
    while (t) {
        digit[len++] = t % 10;
        t /= 10;
    }
    for (int i = 0; i < len; ++i) {
        int d = digit[i];
        if (d == 0) continue;
        uint64_t p = SMALL_PRIMES[i];
        uint64_t pd = powmod_uint64(p, d, orig);
        prod = mul_mod_uint64(prod, pd, orig);
        if (prod == 0) return true;   /* early exit */
    }
    return prod == 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_cnt = 0;
    static size_t cache_cap = 0;
    static uint64_t next_val = 1;   /* next candidate to test */

    if (!cache) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_cnt < n) {
        while (1) {
            uint64_t v = next_val;
            if (good_uint64(v)) {
                if (cache_cnt == cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_cnt++] = (seq_t)v;
                ++next_val;
                break;
            }
            ++next_val;
        }
    }
    return cache[n - 1];
}
