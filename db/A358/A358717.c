/*
 * A358717  A sequence of sorted primes 2 = p_1 < p_2 < ... < p_m such that
 *          (p_i + 1)/2 divides the product p_1*p_2*...*p_(i-1) of the earlier
 *          primes and each prime factor of (p_i-1)/2 is a prime factor of the
 *          product.
 * https://oeis.org/A358717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A358717: sequence of sorted primes p_i such that
 *   (p_i + 1)/2 divides p_1 * … * p_(i-1) and
 *   each prime factor of (p_i – 1)/2 occurs among earlier primes.
 *
 * The algorithm builds the sequence by successively testing odd
 * candidates.  It factors (p±1)/2 with a 64‑bit Pollard‑Rho routine
 * and keeps a sorted array of previously found primes for membership
 * checks.  The sequence is known to terminate after 13 terms; the
 * routine stops at that point and returns 0 for larger indices.
 *
 * The implementation is fully portable C99/C++17 and uses only the
 * provided utilities through internal helper functions.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd64.h"

/* ---------- small primes for trial division ---------- */
static const uint64_t SMALL_PRIMES[] = {
    2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,
    31ULL,37ULL,41ULL,43ULL,47ULL,53ULL,59ULL,61ULL,67ULL,71ULL,
    73ULL,79ULL,83ULL,89ULL,97ULL
};
static const size_t SMALL_PRIME_COUNT = sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]);

/* ---------- modular arithmetic (64‑bit) ---------- */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod) {
#ifdef __SIZEOF_INT128__
    return (unsigned __int128)a * b % mod;
#else
    uint64_t res = 0;
    while (b) {
        if (b & 1) {
            res += a;
            if (res >= mod) res -= mod;
        }
        a = (a << 1);
        if (a >= mod) a -= mod;
        b >>= 1;
    }
    return res;
#endif
}
static inline uint64_t mod_pow64(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mod_mul64(r, a, mod);
        a = mod_mul64(a, a, mod);
        d >>= 1;
    }
    return r;
}

/* ---------- 64‑bit primality test ---------- */

static bool is_prime64(uint64_t n) {
    if (n < 2) return false;
    for (size_t i = 0; i < SMALL_PRIME_COUNT; ++i) {
        uint64_t p = SMALL_PRIMES[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool ok = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n-1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

/* ---------- Pollard Rho factorisation (64‑bit) ---------- */
static uint64_t pollard_rho(uint64_t n) {
    if ((n & 1ULL) == 0) return 2ULL;
    static uint64_t c = 1ULL;
    while (1) {
        uint64_t x = 2ULL, y = 2ULL, d = 1ULL;
        while (d == 1ULL) {
            x = (mod_mul64(x, x, n) + c) % n;
            y = (mod_mul64(y, y, n) + c) % n;
            y = (mod_mul64(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd64(diff, n);
        }
        if (d != n) return d;
        ++c;
    }
}
static void factor_rec(uint64_t n, uint64_t *fac, size_t *cnt) {
    if (n == 1ULL) return;
    if (is_prime64(n)) { fac[(*cnt)++] = n; return; }
    uint64_t d = pollard_rho(n);
    factor_rec(d, fac, cnt);
    factor_rec(n/d, fac, cnt);
}
static int cmp_uint64(const void *a, const void *b) {
    uint64_t va = *(const uint64_t*)a, vb = *(const uint64_t*)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* ---------- dynamic array for the sequence ---------- */
static seq_t *SEQ   = NULL;      /* array of accumulated primes */
static size_t SEQ_LEN = 0;       /* number of primes stored */
static size_t SEQ_CAP = 0;       /* current capacity */
static bool SEQ_INITED = false; /* whether initial prime 2 added */
static uint64_t LAST_CAND = 3;   /* next odd candidate to test */

/* resize and append element */
static void seq_push_back(seq_t val) {
    if (SEQ_LEN + 1 > SEQ_CAP) {
        size_t newcap = SEQ_CAP ? SEQ_CAP << 1 : 8;
        seq_t *tmp = (seq_t *)realloc(SEQ, newcap * sizeof(seq_t));
        if (!tmp) exit(1);   /* abort if memory allocation fails */
        SEQ = tmp;
        SEQ_CAP = newcap;
    }
    SEQ[SEQ_LEN++] = val;
}

/* binary search in the sorted partial sequence */
static bool seq_contains(seq_t val) {
    size_t lo = 0, hi = SEQ_LEN;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (SEQ[mid] < val)
            lo = mid + 1;
        else if (SEQ[mid] > val)
            hi = mid;
        else
            return true;
    }
    return false;
}

/* test whether candidate prime p satisfies the two conditions */
static bool candidate_ok(uint64_t p) {
    /* factor (p+1)/2 -------------------------------------------------- */
    uint64_t h_plus = (p + 1) >> 1;
    uint64_t fac1[64]; size_t cnt1 = 0;
    factor_rec(h_plus, fac1, &cnt1);
    if (cnt1) {
        qsort(fac1, cnt1, sizeof(uint64_t), cmp_uint64);
        for (size_t i = 0; i < cnt1; ++i) {
            if (i > 0 && fac1[i] == fac1[i-1]) return false; /* not square‑free */
            if (!seq_contains((seq_t)fac1[i])) return false;
        }
    }

    /* factor (p-1)/2 -------------------------------------------------- */
    uint64_t h_minus = (p - 1) >> 1;
    uint64_t fac2[64]; size_t cnt2 = 0;
    factor_rec(h_minus, fac2, &cnt2);
    if (cnt2) {
        qsort(fac2, cnt2, sizeof(uint64_t), cmp_uint64);
        for (size_t i = 0; i < cnt2; ++i) {
            if (i > 0 && fac2[i] == fac2[i-1]) continue; /* repeats allowed */
            if (!seq_contains((seq_t)fac2[i])) return false;
        }
    }
    return true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* safety for offsets <1 */

    /* initialise with first term 2 once */
    if (!SEQ_INITED) {
        seq_push_back((seq_t)2);
        SEQ_INITED = true;
        LAST_CAND = 3;
    }

    /* generate terms until we reach the requested index or the
       known termination at 13 terms. */
    while (SEQ_LEN < (size_t)n && SEQ_LEN < 13) {
        for (;;) {
            uint64_t p = LAST_CAND;
            LAST_CAND += 2;               /* next odd candidate */
            if (!is_prime64(p)) continue; /* skip non‑primes */
            if (candidate_ok(p)) {
                seq_push_back((seq_t)p);
                break;                    /* next term found */
            }
        }
    }

    /* if n is larger than the computed length (i.e. >13),
       the sequence is undefined; return 0 in that case. */
    return (SEQ_LEN >= (size_t)n) ? SEQ[n-1] : (seq_t)0;
}
