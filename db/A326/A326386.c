/*
 * A326386  Non-oblong composites m such that beta(m) = tau(m)/2 - 1 where
 *          beta(m) is the number of Brazilian representations of m and tau(m)
 *          is the number of divisors of m.
 * https://oeis.org/A326386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 144, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_long_representation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A326386: Non‑oblong composites m that have no Brazilian representations of
 * length ≥3 and satisfy tau(m) = 2 ⋅ (beta(m)+1);
 * computed by brute‑force search over successive integers.
 * The function returns the n‑th term of the sequence, with a(1)=8.   */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_long_representation.h"

/* ---------- utility functions ---------- */

/* integer square root, floor of sqrt(n) for uint64_t */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = (n >> 1) + 1;
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  low  = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

/* primality test by trial division (fast enough for the range needed) */
static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return false;
    return true;
}

/* check if m is pronic (oblong) : m = k(k+1) for some k */
static bool is_oblong(uint64_t m)
{
    uint64_t k = isqrt_u64(m);
    if (k * (k + 1) == m) return true;
    if (k > 0 && (k - 1) * k == m) return true;
    return false;
}

/* factorisation of n into primes and exponents (max 16 distinct primes) */
static void factor_u64(uint64_t n, uint64_t *pr, int *exp, size_t *cnt)
{
    *cnt = 0;
    if (n % 2 == 0) {
        int e = 0;
        while (n % 2 == 0) { n /= 2; e++; }
        pr[(*cnt)] = 2; exp[(*cnt)] = e; (*cnt)++;
    }
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; e++; }
            pr[(*cnt)] = p; exp[(*cnt)] = e; (*cnt)++;
        }
    }
    if (n > 1) { pr[(*cnt)] = n; exp[(*cnt)] = 1; (*cnt)++; }
}

/* generate all positive divisors of n using its factorisation.
 * pr[0..pf_cnt-1], exp[0..pf_cnt-1] contain the factors. */
static void gen_divisors_recursive(const uint64_t *pr, const int *exp,
                                   size_t pf_cnt, size_t pos,
                                   uint64_t current, uint64_t *out,
                                   size_t *outcnt)
{
    if (pos == pf_cnt) { out[(*outcnt)++] = current; return; }
    uint64_t mult = 1;
    for (int e = 0; e <= exp[pos]; ++e) {
        gen_divisors_recursive(pr, exp, pf_cnt, pos + 1,
                               current * mult, out, outcnt);
        mult *= pr[pos];
    }
}

/* ---------- memoised sequence generation ---------- */

/* cache for computed terms */
static seq_t *cache    = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static uint64_t last_candidate = 1;        /* last integer examined */

/* ensure cache can hold at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* test whether a given integer m satisfies the sequence conditions */
static bool qualifies(uint64_t m)
{
    if (m < 8) return false;          /* the smallest term is 8 */
    if (is_prime(m)) return false;
    if (is_oblong(m)) return false;

    /* factorisation */
    uint64_t pr[16];
    int exp[16];
    size_t pf_cnt;
    factor_u64(m, pr, exp, &pf_cnt);

    /* divisor count τ(m) */
    unsigned long long tau = 1;
    for (size_t i = 0; i < pf_cnt; ++i) tau *= (unsigned long long)(exp[i] + 1);

    /* generate all divisors */
    uint64_t divisors[2048];
    size_t divcnt = 0;
    gen_divisors_recursive(pr, exp, pf_cnt, 0, 1,
                           divisors, &divcnt);

    /* count length‑2 Brazilian representations with digit d ≥ 2 */
    unsigned long long beta2 = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d = divisors[i];
        if (d < 2) continue;                 /* digit must be ≥2 */
        uint64_t base = m / d - 1;
        if (base >= 2 && d <= base - 1) beta2++;
    }
    if (beta2 == 0) return false;           /* at least one length‑2 representation needed */

    /* check for any representation with length ≥3 */
    if (has_long_representation(m)) return false;

    /* verify τ(m) = 2 ⋅ (beta2 + 1) */
    if (tau != 2ULL * (beta2 + 1ULL)) return false;

    return true; /* m is the desired term */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but keep defined */
    while (cache_size < (size_t)n) {
        uint64_t cand = last_candidate + 1;
        last_candidate = cand;
        if (qualifies(cand)) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = (seq_t)cand;
        }
    }
    return cache[n - 1];
}
