/*
 * A332217  Numbers k for which the 2-adic valuation of sigma(k) is zero and
 *          its 3-adic valuation is 1 (so that sigma(k) is an odd multiple of
 *          3, but not of 9).
 * https://oeis.org/A332217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 196, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A332217 – Numbers k with v2(σ(k))=0 and v3(σ(k))=1
 *  (σ(k) odd, divisible by 3 but not 9)
 *
 *  The algorithm generates numbers of the form s² and 2·s² in
 *  ascending order and keeps those for which σ(k) ≡ 3 or 6 (mod 9).
 *  Prime factorisation of s is used to compute σ(k) modulo 9.
 *
 *  The sequence is memoised; any order of calls to a(n) is supported.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ---------- prime sieve ---------- */
static uint32_t *primes = NULL;
static size_t primes_count = 0;

static void init_primes(void)
{
    if (primes_count > 0) return;          /* already initialised */

    const int SIEVE_LIMIT = 1000000;       /* enough for s up to ≈1e6 */
    bool *is_comp = (bool *)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_comp) return;
    for (int i = 0; i <= SIEVE_LIMIT; ++i) is_comp[i] = false;

    for (int i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (int j = i * i; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = true;
        }
    }

    size_t cnt = 0;
    for (int i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_comp[i]) ++cnt;

    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (int i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_comp[i]) primes[idx++] = (uint32_t)i;

    primes_count = cnt;
    free(is_comp);
}

/* ---------- compute σ(k) modulo 9 for k = s² or 2·s² ---------- */
static bool satisfies_from_s(uint64_t s, bool is_twice)
{
    uint64_t rem = s;
    int mod9 = 1;

    /* factor out 2's */
    unsigned e2 = 0;
    while ((rem & 1ULL) == 0) {       /* rem % 2 == 0 */
        rem >>= 1;
        ++e2;
    }
    unsigned exp2 = e2 * 2 + (is_twice ? 1 : 0);
    if (exp2 > 0) {
        int pow = 1 % 9;
        int sum = 1;                  /* 2⁰ */
        int pmod = 2 % 9;
        for (unsigned i = 0; i < exp2; ++i) {
            pow = (pow * pmod) % 9;
            sum = (sum + pow) % 9;
        }
        mod9 = (mod9 * sum) % 9;
    }

    /* odd prime factors */
    for (size_t pi = 0; pi < primes_count; ++pi) {
        uint32_t p = primes[pi];
        if ((uint64_t)p * p > rem) break;
        if (rem % p == 0) {
            unsigned ep = 0;
            while (rem % p == 0) {
                rem /= p;
                ++ep;
            }
            unsigned exp = ep * 2;
            if (exp == 0) continue;
            int pow = 1 % 9;
            int sum = 1;
            int pmod = p % 9;
            for (unsigned i = 0; i < exp; ++i) {
                pow = (pow * pmod) % 9;
                sum = (sum + pow) % 9;
            }
            mod9 = (mod9 * sum) % 9;
        }
    }

    if (rem > 1) {                     /* remaining odd prime */
        int pmod = rem % 9;
        int pow = 1 % 9;
        int sum = 1;                   /* p⁰ */
        for (unsigned i = 0; i < 2; ++i) {   /* exponent 2 in k */
            pow = (pow * pmod) % 9;
            sum = (sum + pow) % 9;
        }
        mod9 = (mod9 * sum) % 9;
    }

    return (mod9 == 3 || mod9 == 6);
}

/* ---------- memoised sequence cache ---------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                    /* undefined for n=0 */

    init_primes();

    static uint64_t sq_ptr = 1;           /* current s for squares   */
    static uint64_t tw_ptr = 1;           /* current s for 2·s²      */

    while (cache_len < (size_t)n) {
        /* next candidates: s² and 2·s² */
        uint64_t s_sq = sq_ptr;
        __uint128_t cand_sq128 = (__uint128_t)s_sq * s_sq;
        uint64_t cand_sq = (uint64_t)cand_sq128;

        uint64_t s_tw = tw_ptr;
        __uint128_t cand_tw128 = (__uint128_t)2 * s_tw * s_tw;
        uint64_t cand_tw = (uint64_t)cand_tw128;

        uint64_t next_val;
        bool is_twice;
        if (cand_sq < cand_tw) {
            next_val = cand_sq;
            is_twice = false;
            ++sq_ptr;
        } else {
            next_val = cand_tw;
            is_twice = true;
            ++tw_ptr;
        }

        if (satisfies_from_s(is_twice ? tw_ptr - 1 : sq_ptr - 1, is_twice)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;      /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_val;
        }
    }

    return cache[n - 1];
}
