/*
 * A272895  a(n) is the largest natural number k such that the composite
 *          number (2n+1) 2^k+1 has a nontrivial divisor of the form h2^s+1 (h
 *          odd) with 2s >= k. If such a natural number does not exist, we set
 *          a(n)=0.
 * https://oeis.org/A272895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the largest k such that the composite number
 *            (2n+1)·2^k+1 has a proper divisor of the form h·2^s+1 with h odd
 *            and 2s ≥ k; if no such k exists a(n)=0
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/gcd64.h"

/* ----------  64‑bit modular arithmetic  ---------- */

static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* ----------  GCD  ---------- */

/* ----------  Miller-Rabin primality test (deterministic for 64‑bit)  ---------- */

static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t smallp[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(smallp)/sizeof(smallp[0]); ++i) {
        if (n % smallp[i] == 0) return n == smallp[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n-1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ----------  Pollard's Rho factorisation  ---------- */

static uint64_t pollard_rho(uint64_t n)
{
    if (n % 2ULL == 0ULL) return 2ULL;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mul_mod(x, x, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            d = gcd64((x > y) ? x - y : y - x, n);
        }
        if (d != n) return d;
        ++c;
    }
}

/* Recursive factorisation.  The array `fac` must have room for at least
   64 entries; `cnt` will contain the number of recorded prime factors
   (possibly repeated). */
static void factor_uint64(uint64_t n, uint64_t *fac, size_t *cnt)
{
    if (n == 1) return;
    if (is_prime_64(n)) { fac[(*cnt)++] = n; return; }
    uint64_t d = pollard_rho(n);
    factor_uint64(d, fac, cnt);
    factor_uint64(n / d, fac, cnt);
}

/* ----------  Cooperative helper prototypes  ---------- */

static int cmp_uint64(const void *a, const void *b);
static bool rec_divisors(size_t idx, uint64_t cur, uint64_t M,
                         uint64_t targetK,
                         uint64_t *pr, size_t *ex, size_t cnt);

/* ----------  Compare function for qsort  ---------- */

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x < y) ? -1 : ((x > y) ? 1 : 0);
}

/* ----------  Recursive divisor generator  ---------- */

static bool rec_divisors(size_t idx, uint64_t cur, uint64_t M,
                         uint64_t targetK,
                         uint64_t *pr, size_t *ex, size_t cnt)
{
    if (idx == cnt) {
        if (cur == 1 || cur == M) return false;
        uint64_t t = cur - 1;
        size_t s = 0;
        while ((t & 1ULL) == 0) { t >>= 1; ++s; }
        return (2 * s >= targetK);
    }
    uint64_t p = pr[idx];
    uint64_t pow = 1;
    for (size_t e = 0; e <= ex[idx]; ++e) {
        if (rec_divisors(idx + 1, cur * pow, M, targetK, pr, ex, cnt))
            return true;
        if (e == ex[idx]) break;
        pow *= p;
    }
    return false;
}

/* ----------  Main sequence function  ---------- */

seq_t a(idx_t n)
{
    /* 2*n+1 as idx_t (64‑bit unsigned) */
    idx_t N = n * 2ULL + 1ULL;

    /* Find maximum k such that (2n+1)·2^k+1 fits in 64 bit. */
    unsigned maxK = 0;
    while ( ( (__uint128_t)N << maxK ) <= ( (__uint128_t)UINT64_MAX - 1ULL ) )
        ++maxK;

    /* Try k from largest downwards. */
    for (int k = (int)maxK - 1; k >= 0; --k) {
        __uint128_t temp = (__uint128_t)N << k;
        temp += 1ULL;                    /* M = (2n+1)·2^k+1 */
        uint64_t M = (uint64_t)temp;
        if (is_prime_64(M)) continue;    /* composite required */

        /* Factor M. */
        uint64_t facs[64];
        size_t facCnt = 0;
        factor_uint64(M, facs, &facCnt);

        /* Collapse prime multiplicities for divisor generation. */
        if (facCnt == 0) continue;       /* should not happen */

        qsort(facs, facCnt, sizeof(uint64_t), cmp_uint64);

        uint64_t uniq[64];
        size_t uniqExp[64];
        size_t uniqCnt = 0;
        for (size_t i = 0; i < facCnt; ) {
            size_t j = i;
            while (j < facCnt && facs[j] == facs[i]) ++j;
            uniq[uniqCnt] = facs[i];
            uniqExp[uniqCnt] = j - i;
            ++uniqCnt;
            i = j;
        }

        if (rec_divisors(0, 1ULL, M, (uint64_t)k,
                         uniq, uniqExp, uniqCnt))
            return (seq_t)k;
    }
    return (seq_t)0;
}
