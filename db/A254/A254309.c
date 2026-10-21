/*
 * A254309  Irregular triangular array read by rows: T(n,k) is the least
 *          positive primitive root of the n-th prime p=prime(n) raised to
 *          successive powers of k (mod p) where 1<=k<=p-1 and gcd(k,p-1)=1.
 * https://oeis.org/A254309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 2420, u16 n <= 9926, u32 n <= 9926, u64 n <= 9926,
 *            u128 n <= 9926, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is obtained by flattening the irregular triangular array
   described in OEIS A254309.  It repeatedly generates successive primes,
   finds each prime’s least primitive root g, and for each
   k coprime to p-1 (in ascending order) records g^k mod p.
   The index n is mapped to the appropriate row and column, and the
   required value is returned.  The algorithm uses a cache so that calls
   arrive in arbitrary order. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/gcd.h"          /* for gcd(seq_t a, seq_t b) */
#include "../utils/mod_pow.h"      /* for mod_pow(seq_t base, uint64_t exp, seq_t mod) */

typedef idx_t idx_t;
typedef seq_t seq_t;

/* ---------- small utilities ---------- */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0ULL) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ensure array has at least 'need' capacity, reallocating if necessary */
static void ensure_capacity(size_t need, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        if (new_cap < need) new_cap = need;
        void *tmp = realloc(*ptr, new_cap * elem_size);
        if (!tmp) exit(EXIT_FAILURE);
        *ptr = tmp;
        *cap = new_cap;
    }
}

/* ---------- data structures for primes and rows ---------- */
typedef struct {
    uint64_t p;      /* prime */
    seq_t   g0;      /* least primitive root of p */
    size_t  len;     /* phi(p-1) */
} PrimeRow;

/* global tables */
static size_t prime_cap    = 0;
static size_t prime_cnt    = 0;
static uint64_t *primes    = NULL;   /* holds all primes in order */

static size_t rows_cap     = 0;
static size_t rows_cnt     = 0;
static PrimeRow *rows      = NULL;   /* table of prime rows */

static size_t cum_cap      = 0;
static size_t cum_cnt      = 0;      /* includes sentinel 0 */
static size_t *cum         = NULL;   /* cum[0] = 0, cum[i] = total terms up to row i-1 */

/* ---------- prime test using known primes ---------- */
static bool is_prime_candidate(uint64_t x)
{
    if (x < 2ULL) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0ULL) return false;
    }
    return true;
}

/* factorize n into distinct prime factors, result in fact[] (size >= 32) */
static void factor_unique(uint64_t n, uint64_t fact[32], size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0ULL) {
            fact[(*cnt)++] = p;
            while (n % p == 0ULL) n /= p;
        }
    }
    if (n > 1ULL) fact[(*cnt)++] = n;
}

/* ---------- add the next prime and its row information ---------- */
static void add_next_prime(void)
{
    /* find next prime */
    uint64_t candidate = (prime_cnt == 0) ? 2ULL : primes[prime_cnt - 1] + 1ULL;
    while (!is_prime_candidate(candidate)) ++candidate;

    /* store prime */
    ensure_capacity(prime_cnt + 1, &prime_cap, (void **)&primes, sizeof(uint64_t));
    primes[prime_cnt++] = candidate;

    /* ---------- compute phi(p-1) and its prime factors ---------- */
    uint64_t factors[32];
    size_t factor_cnt;
    factor_unique(candidate - 1ULL, factors, &factor_cnt);

    size_t phi = candidate - 1ULL;
    for (size_t i = 0; i < factor_cnt; ++i) {
        phi -= phi / factors[i];
    }

    /* ---------- find least primitive root g0 ---------- */
    seq_t g0;
    if (candidate == 2ULL) {
        /* only element 1 is a primitive root of 2 */
        g0 = (seq_t)1;
    } else {
        g0 = (seq_t)0;
        for (uint64_t g = 2ULL; g < candidate; ++g) {
            bool ok = true;
            for (size_t i = 0; i < factor_cnt; ++i) {
                uint64_t exp = (candidate - 1ULL) / factors[i];
                if (mod_pow((seq_t)g, exp, (seq_t)candidate) == (seq_t)1) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                g0 = (seq_t)g;
                break;
            }
        }
    }

    /* ---------- store row data ---------- */
    size_t row_idx = rows_cnt;
    ensure_capacity(rows_cnt + 1, &rows_cap, (void **)&rows, sizeof(PrimeRow));
    rows[row_idx].p   = candidate;
    rows[row_idx].g0  = g0;
    rows[row_idx].len = phi;
    rows_cnt++;

    /* ---------- update cumulative counts ---------- */
    if (cum_cnt == 0) {
        /* sentinel 0 */
        ensure_capacity(1, &cum_cap, (void **)&cum, sizeof(size_t));
        cum[0] = 0;
        cum_cnt = 1;
    }
    /* append new cumulative value */
    ensure_capacity(cum_cnt + 1, &cum_cap, (void **)&cum, sizeof(size_t));
    cum[cum_cnt] = cum[cum_cnt - 1] + rows[row_idx].len;
    cum_cnt++;
}

/* ---------- ensure rows cover at least the target index (1‑based) ---------- */
static void ensure_up_to_idx(size_t target)
{
    while (cum_cnt == 0 || cum[cum_cnt - 1] < target)
        add_next_prime();
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety, offset is 1 */

    size_t target = (size_t)n;            /* n >= 1 */

    ensure_up_to_idx(target);

    /* find row r such that cum[r] >= target, cum[0] = 0 */
    size_t r = 1;
    while (cum[r] < target) ++r;          /* cum[r] is cumulative after row r-1 */

    size_t row_idx = r - 1;               /* zero‑based row index */
    uint64_t p     = rows[row_idx].p;
    seq_t   g0     = rows[row_idx].g0;
    size_t  row_len = rows[row_idx].len;
    size_t  offset  = target - cum[r - 1] - 1;   /* 0‑based in the row */

    /* find the offset‑th integer k coprime to p-1 (ascending order) */
    uint64_t k = 0;
    size_t cnt = 0;
    for (uint64_t cand = 1ULL; cand < p; ++cand) {
        if (gcd_uint64(cand, p - 1ULL) != 1ULL) continue;
        if (cnt == offset) { k = cand; break; }
        ++cnt;
    }

    /* compute g0^k mod p */
    seq_t val = mod_pow(g0, k, (seq_t)p);
    return val;
}
