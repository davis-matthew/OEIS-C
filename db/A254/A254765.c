/*
 * A254765  Fundamental positive solution y = y1(n) of the first class of the
 *          Pell equation x^2 - 2*y^2 = A007522(n), n >=1 (primes congruent to
 *          7 mod 8).
 * https://oeis.org/A254765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A254765: y = fundamental positive solution of x^2 - 2*y^2 = p,
 * where p is the n‑th prime congruent to 7 (mod 8).  The values y
 * are computed by iterating y upward until p + 2*y² is a perfect square.
 * The function caches computed y values and the list of primes ≡ 7 (mod 8).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------------------------- Integer square root ---------------------------- */

static uint64_t uint64_isqrt(uint64_t v)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;            /* sqrt(2^64−1) < 2^32 */
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        __int128 sq = (__int128)mid * (__int128)mid;
        if (sq <= (__int128)v)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* ------------------------- Primes ≡ 7 (mod 8) ------------------------------ */

#define SIEVE_LIMIT 700000          /* ≥ 10 000 primes of the desired form */

static uint64_t *primes7 = NULL;   /* 0‑based array of primes */
static size_t primes7_cnt = 0;
static bool primes7_inited = false;

static void init_primes7(void)
{
    if (primes7_inited) return;
    primes7_inited = true;

    size_t size = SIEVE_LIMIT + 1;
    bool *is_prime = (bool *)calloc(size, sizeof(bool));
    if (!is_prime) return;                 /* out of memory – unlikely */

    for (size_t i = 2; i < size; ++i) is_prime[i] = true;
    for (size_t p = 2; p * p < size; ++p) {
        if (is_prime[p]) {
            for (size_t j = p * p; j < size; j += p) is_prime[j] = false;
        }
    }

    /* Count primes  ≡ 7 (mod 8) */
    for (size_t i = 2; i < size; ++i)
        if (is_prime[i] && (i & 7) == 7) ++primes7_cnt;

    primes7 = (uint64_t *)malloc(primes7_cnt * sizeof(uint64_t));
    if (!primes7) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i < size; ++i)
        if (is_prime[i] && (i & 7) == 7) primes7[idx++] = (uint64_t)i;

    free(is_prime);
}

/* ---------------------------- Pell solver -------------------------------- */

static seq_t compute_y_for_prime(uint64_t p)
{
    for (seq_t y = 1; ; ++y) {
        uint64_t ysq = (uint64_t)y * (uint64_t)y;          /* y²      */
        uint64_t rhs = p + 2ULL * ysq;                    /* x²      */
        uint64_t x   = uint64_isqrt(rhs);
        if (x * x == rhs) return y;                      /* solution found */
    }
}

/* --------------------------- Cache handling ------------------------------- */

static seq_t *y_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_used = 0;

/* enlarge cache capacity if required */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = needed >= 2 ? needed * 2 : 2;
    seq_t *tmp = (seq_t *)realloc(y_cache, new_cap * sizeof(seq_t));
    if (tmp) {
        y_cache = tmp;
        cache_cap = new_cap;
    }
}

/* ----------------------------- The sequence ------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                         /* below offset */

    init_primes7();
    if (primes7_cnt < (size_t)n) return (seq_t)0;       /* safety guard */

    /* Compute any missing entries up to n */
    if ((size_t)n > cache_used) {
        if (n > cache_cap) ensure_cache_capacity((size_t)n);
        for (size_t i = cache_used; i < (size_t)n; ++i) {
            uint64_t p = primes7[i];
            y_cache[i] = compute_y_for_prime(p);
        }
        cache_used = (size_t)n;
    }

    return y_cache[(size_t)(n - 1)];
}
