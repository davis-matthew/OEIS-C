/*
 * A253253  a(n) = smallest divisor of the concatenation of n and n+1 that did
 *          not occur earlier.
 * https://oeis.org/A253253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = smallest divisor of concat(n,n+1) that has not appeared
 *            for any m < n. The function computes all previous terms on
 *            demand and stores them in static arrays so that subsequent calls
 *            are fast
 * Range:     u8 n <= 8, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/concat_parts.h"

static uint64_t *prime_list = NULL;
static idx_t prime_cnt = 0;
static bool primes_ready = false;

static void generate_primes(void)
{
    if (primes_ready) return;
    const idx_t LIMIT = 200000;                 /* enough for n < 10^4 */
    prime_list = (uint64_t *)malloc(LIMIT * sizeof(uint64_t));
    bool *sieve   = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    for (idx_t i = 0; i <= LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = false;
    prime_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) prime_list[prime_cnt++] = (uint64_t)i;
    free(sieve);
    primes_ready = true;
}

static void ensure_capacity(size_t *cap, void **ptr, size_t elem, size_t needed)
{
    if (*cap >= needed) return;
    size_t newcap = *cap ? *cap * 2 : 64;
    while (newcap < needed) newcap *= 2;
    *ptr = realloc(*ptr, newcap * elem);
    *cap = newcap;
}

/* Storage for all computed a(k) and for the set of seen values */
static seq_t *a_cache   = NULL;
static size_t a_cap = 0, a_used = 0;
static seq_t *seen_cache = NULL;
static size_t seen_cap = 0, seen_used = 0;

/* Linear search; enough for n up to ~10^4 */
static bool seen_contains(seq_t v)
{
    for (size_t i = 0; i < seen_used; ++i)
        if (seen_cache[i] == v) return true;
    return false;
}
static void seen_add(seq_t v)
{
    if (seen_contains(v)) return;
    ensure_capacity(&seen_cap, (void **)&seen_cache, sizeof(seq_t), seen_used + 1);
    seen_cache[seen_used++] = v;
}

/* Recursive divisor enumeration: updates d_min with smallest fresh divisor */
static void dfs_div(const uint64_t *primes, const int *exps, int num,
                    uint64_t cur, seq_t *d_min)
{
    if (num == 0) {
        seq_t d = (seq_t)cur;
        if (!seen_contains(d))
            if (*d_min == 0 || d < *d_min) *d_min = d;
        return;
    }
    const uint64_t p = primes[0];
    int emax = exps[0];
    uint64_t mul = 1;
    for (int e = 0; e <= emax; ++e) {
        dfs_div(primes + 1, exps + 1, num - 1, cur * mul, d_min);
        mul *= p;
    }
}

/* Compute a(n), computing all previous terms if necessary */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, but guard */
    generate_primes();

    /* Compute missing terms */
    while (a_used < n) {
        idx_t k = a_used + 1; /* term number */
        seq_t concat_val = concat_parts(k, k + 1);
        uint64_t m = (uint64_t)concat_val; /* fits in 64-bit for n<10000 */

        /* factorise m */
        uint64_t fac[64];
        int exp[64];
        int fcnt = 0;
        uint64_t tmp = m;
        for (idx_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
            uint64_t p = prime_list[i];
            if (tmp % p == 0) {
                int e = 0;
                while (tmp % p == 0) { tmp /= p; ++e; }
                fac[fcnt] = p; exp[fcnt] = e; ++fcnt;
            }
        }
        if (tmp > 1) { fac[fcnt] = tmp; exp[fcnt] = 1; ++fcnt; }

        seq_t min_candidate = 0;
        dfs_div(fac, exp, fcnt, 1, &min_candidate);
        if (min_candidate == 0) min_candidate = 1; /* safety */

        /* store */
        ensure_capacity(&a_cap, (void **)&a_cache, sizeof(seq_t), a_used + 1);
        a_cache[a_used++] = min_candidate;
        seen_add(min_candidate);
    }
    return a_cache[n - 1];
}
