/*
 * A398333  Number of integers r, 1<=r<n, such that L(n*r) < L(n*r+1) where
 *          L(x) is the largest divisor of x <= the square root of x.
 * https://oeis.org/A398333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{1 ≤ r < n : L(n*r) < L(n*r+1)}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- Integer square root (floor) for 64‑bit unsigned integers ---------- */
static inline uint64_t isqrt_u64(uint64_t x) {
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1 << 62;          /* largest power of four ≤ x */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ---------- Helper to fill an array of primes up to LIMIT (≤ 10000) ---------- */
#define PRIMELIMIT 10000
#define MAX_PRIMES 2000

static void init_primes(uint32_t *primes, size_t *cnt) {
    static bool is_comp[PRIMELIMIT + 1];          /* zero‑initialised static */
    *cnt = 0;
    for (uint32_t p = 2; p <= PRIMELIMIT; ++p) {
        if (!is_comp[p]) {
            primes[*cnt] = p;
            ++(*cnt);
            if ((uint64_t)p * p <= PRIMELIMIT) {
                for (uint32_t q = p * p; q <= PRIMELIMIT; q += p)
                    is_comp[q] = true;
            }
        }
    }
}

/* ---------- Recursively search all divisors up to lim ---------- */
static void dfs_divs(const uint64_t *facs, const int *exps,
                     size_t total, size_t idx, uint64_t cur,
                     uint64_t lim, uint64_t *best) {
    if (idx == total) {
        if (cur > *best) *best = cur;
        return;
    }
    uint64_t p = facs[idx];
    uint64_t val = cur;
    for (int e = 0; e <= exps[idx]; ++e) {
        if (val > lim) break;
        dfs_divs(facs, exps, total, idx + 1, val, lim, best);
        if (e < exps[idx]) {
            if (val > lim / p) break;
            val *= p;
        }
    }
}

/* ---------- Largest divisor of x not exceeding sqrt(x) ---------- */
static uint64_t largest_divisor_le_sqrt(uint64_t x,
                                        const uint32_t *primes, size_t prime_cnt) {
    if (x <= 1) return x;   /* L(1)=1, L(0) is never requested */
    uint64_t lim = isqrt_u64(x);
    uint64_t n = x;
    uint64_t fac[20];
    int exp[20];
    size_t cnt = 0;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        if (n % primes[i] == 0) {
            int e = 0;
            while (n % primes[i] == 0) {
                n /= primes[i];
                ++e;
            }
            fac[cnt] = primes[i];
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (n > 1) {        /* remaining prime factor */
        fac[cnt] = n;
        exp[cnt] = 1;
        ++cnt;
    }

    uint64_t best = 1;
    dfs_divs(fac, exp, cnt, 0, 1, lim, &best);
    return best;
}

/* ---------- Sequence function a(n) ---------- */
seq_t a(idx_t n) {
    if (n <= 1) return 0;

    static uint32_t primes[MAX_PRIMES];
    static size_t prime_cnt;
    static bool initialized = false;
    if (!initialized) {
        init_primes(primes, &prime_cnt);
        initialized = true;
    }

    uint64_t total = 0;
    for (idx_t r = 1; r < n; ++r) {
        uint64_t prod = (uint64_t)n * (uint64_t)r;
        uint64_t l1 = largest_divisor_le_sqrt(prod, primes, prime_cnt);
        uint64_t l2 = largest_divisor_le_sqrt(prod + 1, primes, prime_cnt);
        if (l1 < l2) ++total;
    }
    return (seq_t)total;
}
