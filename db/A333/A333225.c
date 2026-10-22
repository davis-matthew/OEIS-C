/*
 * A333225  Position of first appearance of n in A333226 (LCMs of compositions
 *          in standard order).
 * https://oeis.org/A333225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ∑ 2^{(∑_{j≤i} p_j)−1} where p_j are the prime powers in
 *            the factorization of n, sorted increasingly; a(1)=1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static inline uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod) {
    __int128 t = (__int128)a * b;
    return (uint64_t)(t % mod);
}

static inline uint64_t powmod(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mulmod(r, a, mod);
        a = mulmod(a, a, mod);
        e >>= 1;
    }
    return r;
}

static inline uint64_t gcd_uint64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Deterministic Miller–Rabin for 64‑bit integers */
static inline bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    /* bases sufficient for 64‑bit */
    static const uint64_t bases[] = {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mulmod(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Pollard Rho with deterministic fallback */
static inline uint64_t pollard_rho(uint64_t n) {
    if ((n & 1) == 0) return 2;
    uint64_t c = 1;
    uint64_t x = 2, y = 2, d = 1;
    while (d == 1) {
        x = (mulmod(x, x, n) + c) % n;
        y = (mulmod(y, y, n) + c) % n;
        y = (mulmod(y, y, n) + c) % n;
        d = gcd_uint64((x>y?x-y:y-x), n);
        if (d == n) { c++; x = 2; y = 2; d = 1; }
    }
    return d;
}

static void factor_rec(uint64_t n, uint64_t *pr, int *ex, size_t *cnt) {
    if (n == 1) return;
    if (is_prime_uint64(n)) {
        pr[*cnt] = n;
        ex[*cnt] = 1;
        (*cnt)++;
        return;
    }
    uint64_t d = pollard_rho(n);
    factor_rec(d, pr, ex, cnt);
    factor_rec(n / d, pr, ex, cnt);
}

static void merge_factors(uint64_t *pr, int *ex, size_t *cnt) {
    if (*cnt <= 1) return;
    /* simple bubble sort by prime */
    for (size_t i = 0; i < *cnt; ++i)
        for (size_t j = i + 1; j < *cnt; ++j)
            if (pr[i] > pr[j]) {
                uint64_t t = pr[i]; pr[i] = pr[j]; pr[j] = t;
                int e = ex[i]; ex[i] = ex[j]; ex[j] = e;
            }
    size_t w = 0;
    for (size_t i = 0; i < *cnt; ++i) {
        if (w == 0 || pr[i] != pr[w-1]) {
            pr[w] = pr[i];
            ex[w] = ex[i];
            ++w;
        } else {
            ex[w-1] += ex[i];
        }
    }
    *cnt = w;
}

static uint64_t pow_uint64(uint64_t base, int exp) {
    uint64_t r = 1;
    while (exp--) r *= base;
    return r;
}

static void sort_ascending(uint64_t *a, size_t n) {
    for (size_t i = 0; i < n; ++i)
        for (size_t j = i + 1; j < n; ++j)
            if (a[i] > a[j]) {
                uint64_t t = a[i]; a[i] = a[j]; a[j] = t;
            }
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    uint64_t primes[64];
    int exps[64];
    size_t cnt = 0;
    factor_rec(n, primes, exps, &cnt);
    merge_factors(primes, exps, &cnt);

    /* Build array of prime powers */
    uint64_t pow_vals[64];
    size_t pw_cnt = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t v = 1;
        for (int j = 0; j < exps[i]; ++j) v *= primes[i];
        pow_vals[pw_cnt++] = v;
    }
    sort_ascending(pow_vals, pw_cnt);

    seq_t result = 0;
    uint64_t cumulative = 0;
    for (size_t i = 0; i < pw_cnt; ++i) {
        cumulative += pow_vals[i];
        result += pow2((idx_t)(cumulative - 1));
    }
    return result;
}
