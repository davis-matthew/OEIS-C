/*
 * A327251  Expansion of Sum_{k>=1} psi(k) * x^k / (1 - x^k)^2, where psi =
 *          A001615.
 * https://oeis.org/A327251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} psi(n/d) * d, where psi is Dedekind psi
 * Range:     u8 n <= 23, u16 n <= 1199, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_uint64.h"

/* ----------  64‑bit arithmetic helpers  ---------- */
static uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mod_mul_uint64(result, base, mod);
        base = mod_mul_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* ----------  deterministic Miller–Rabin for 64‑bit  ---------- */
static int is_prime_uint64(uint64_t n) {
    if (n < 2) return 0;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int cont = 0;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) { cont = 1; break; }
        }
        if (!cont) return 0;
    }
    return 1;
}

/* ----------  Pollard–Rho factorisation  ---------- */
static uint64_t pollard_rho(uint64_t n) {
    if ((n & 1ULL) == 0ULL) return 2ULL;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mod_mul_uint64(x, x, n) + c) % n;
            y = (mod_mul_uint64(y, y, n) + c) % n;
            y = (mod_mul_uint64(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_uint64(diff, n);
        }
        if (d != n) return d;
        ++c; /* try a different constant */
    }
}

/* ----------  recursive factorisation into primes  ---------- */
static void factor_recurse(uint64_t n, uint64_t *pr, int *ex, size_t *cnt) {
    if (n == 1) return;
    if (is_prime_uint64(n)) {
        for (size_t i = 0; i < *cnt; ++i) {
            if (pr[i] == n) { ++ex[i]; return; }
        }
        pr[*cnt] = n; ex[*cnt] = 1; ++(*cnt);
        return;
    }
    uint64_t d = pollard_rho(n);
    factor_recurse(d, pr, ex, cnt);
    factor_recurse(n / d, pr, ex, cnt);
}

/* ----------  fast power for seq_t  ---------- */
static seq_t seq_pow_uint64(seq_t base, uint64_t exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res = res * base;
        base = base * base;
        exp >>= 1;
    }
    return res;
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but safe for n=0 */
    if (n == 1) return 1;

    uint64_t nn = (uint64_t)n;
    /* prime factors: at most 64 distinct primes for 64‑bit integer */
    uint64_t primes[64];
    int exponents[64];
    size_t cnt = 0;
    factor_recurse(nn, primes, exponents, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exponents[i];
        /* compute p^(e-1) as seq_t */
        seq_t part = seq_pow_uint64((seq_t)p, e - 1);
        /* second factor: ((p+1)*e + p) */
        seq_t second = (seq_t)(p + 1) * (seq_t)e;
        second = second + (seq_t)p;
        seq_t term = part * second;
        result = result * term;
    }
    return result;
}
