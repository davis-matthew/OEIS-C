/*
 * A336905  Numbers n such that for any i > 0 there is some j > 0 such that
 *          the prime(i)-adic valuation of n, say x, equals the prime(j)-adic
 *          valuation of n and x = abs(i-j) (where prime(k) denotes the k-th
 *          prime number and the p-adic valuation of a number is the greatest
 *          m such that p^m divides that number).
 * https://oeis.org/A336905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A336905: numbers n such that for every prime p_i dividing n
 * there is a prime p_j dividing n with the same p-adic exponent x,
 * and |i-j| = x, where i and j are the 1‑based indices of the primes
 * among all primes (prime(1) = 2, prime(2) = 3, etc.).  The function
 * generates terms by brute‑force testing of successive integers
 * until the requested index is reached. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* types supplied by the build system */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Simple growing list of primes, used for factorisation and index   */
/* ------------------------------------------------------------------ */

typedef struct {
    uint32_t *data;   /* ascending list of primes */
    size_t   cnt;
    size_t   cap;
} prime_list_t;

static prime_list_t prime_list = {NULL,0,0};

static void prime_list_init(void)
{
    if (prime_list.cap == 0) {
        prime_list.cap = 128;
        prime_list.data = (uint32_t *)malloc(prime_list.cap * sizeof(uint32_t));
    }
    if (prime_list.cnt == 0) {       /* seed with 2 */
        prime_list.data[prime_list.cnt++] = 2;
    }
}

static void prime_list_push(uint32_t p)
{
    if (prime_list.cnt == prime_list.cap) {
        prime_list.cap *= 2;
        prime_list.data = (uint32_t *)realloc(prime_list.data,
                                             prime_list.cap * sizeof(uint32_t));
    }
    prime_list.data[prime_list.cnt++] = p;
}

static int prime_index(uint32_t p)   /* 1‑based index; -1 if not found */
{
    size_t lo = 0, hi = prime_list.cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint32_t v = prime_list.data[mid];
        if (v == p) return (int)(mid + 1);
        if (v < p) lo = mid + 1;
        else hi = mid;
    }
    return -1;
}

/* ------------------------------------------------------------------ */
/* Test whether a candidate satisfies the property                    */
/* ------------------------------------------------------------------ */

#define MAX_EXP 128        /* ample for any 64‑bit exponent */
static int  group_idx[MAX_EXP][64];
static int  group_len[MAX_EXP];

static bool is_valid_candidate(idx_t n)
{
    if (n == 1) return true;

    prime_list_init();

    /* factorise n by trial division using the current prime list   */
    uint64_t m = (uint64_t)n;
    size_t fac_cnt = 0;
    uint32_t fac_p[32];
    uint32_t fac_e[32];

    for (size_t i = 0; i < prime_list.cnt && (uint64_t)prime_list.data[i] * (uint64_t)prime_list.data[i] <= m; ++i) {
        uint32_t p = prime_list.data[i];
        if (m % p == 0) {
            uint32_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            fac_p[fac_cnt] = p;
            fac_e[fac_cnt] = e;
            ++fac_cnt;
        }
    }
    if (m > 1) {                   /* remaining prime factor > sqrt n   */
        uint32_t p = (uint32_t)m;
        if (prime_index(p) == -1) prime_list_push(p); /* keep list sorted    */
        fac_p[fac_cnt] = p;
        fac_e[fac_cnt] = 1;
        ++fac_cnt;
    }

    /* initialise group lengths */
    for (int e = 0; e < MAX_EXP; ++e) group_len[e] = 0;

    /* collect prime indices per exponent */
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint32_t p = fac_p[i];
        uint32_t e = fac_e[i];
        if (e >= (uint32_t)MAX_EXP) return false;   /* too large exponent   */
        int idx_pos = prime_index(p);
        if (idx_pos <= 0) return false;              /* should not happen   */
        int *arr   = group_idx[e];
        int len    = group_len[e];
        if (len >= 64) return false;                 /* too many primes same e */
        arr[len] = idx_pos;
        group_len[e] = len + 1;
    }

    /* validate: each exponent group must have at least two primes,
     * and every prime index i must have a partner j with |i-j| = e
     */
    for (int e = 1; e < MAX_EXP; ++e) {
        int len = group_len[e];
        if (len == 0) continue;
        if (len == 1) return false;                   /* isolated exponent   */
        int *arr = group_idx[e];
        for (int a = 0; a < len; ++a) {
            int i = arr[a];
            bool ok = false;
            for (int b = 0; b < len; ++b) {
                if (a == b) continue;
                int j = arr[b];
                int diff = i - j;
                if (diff < 0) diff = -diff;
                if (diff == e) { ok = true; break; }
            }
            if (!ok) return false;
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Cache of already generated terms                                  */
/* ------------------------------------------------------------------ */

typedef struct {
    seq_t *data;
    size_t cnt;
    size_t cap;
} cache_t;

static cache_t cache = {NULL,0,0};

static void ensure_cache_cap(size_t new_cap)
{
    if (cache.cap >= new_cap) return;
    cache.cap = new_cap;
    cache.data = (seq_t *)realloc(cache.data, cache.cap * sizeof(seq_t));
}

/* ------------------------------------------------------------------ */
/* Sequence entry point                                               */
/* ------------------------------------------------------------------ */

static idx_t last_checked = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for index 0 */

    if (cache.cap == 0) {
        cache.cap = 128;
        cache.data = (seq_t *)malloc(cache.cap * sizeof(seq_t));
    }

    while (cache.cnt < (size_t)n) {
        ++last_checked;
        if (is_valid_candidate(last_checked)) {
            if (cache.cnt == cache.cap) ensure_cache_cap(cache.cap + 1);
            cache.data[cache.cnt++] = (seq_t)last_checked;
        }
    }
    return cache.data[(size_t)(n - 1)];
}
