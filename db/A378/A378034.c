/*
 * A378034  First-differences of A378032 (greatest number < prime(n) that is 1
 *          or nonsquarefree).
 * https://oeis.org/A378034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A378036(prime(n)) * Compute the first difference of the
 *            sequence A378032, the * greatest integer k < p such that k == 1
 *            or k is nonsquarefree, * evaluated at the nth prime
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* External integer types provided by the build system */
extern idx_t _prime_limit_for_idx(idx_t n);
extern bool is_squarefree(seq_t x);

/* --- Cached data shared by all calls to a() --- */
static uint64_t *cached_primes      = NULL;  /* array of primes       */
static size_t    cached_prime_cnt   = 0;     /* number of primes       */
static seq_t    *cached_f          = NULL;  /* f(p) for each prime    */

/* ------------------------------------------------------------------ */
/* Simple Sieve of Eratosthenes up to `limit`. The returned array
 * contains all primes <= limit and its size in out_cnt.            */
static void sieve_primes_upto(uint64_t limit, uint64_t **out, size_t *out_cnt)
{
    size_t sz = (size_t)(limit + 1);
    bool *is_comp = (bool*)malloc(sz);
    if (!is_comp) { *out = NULL; *out_cnt = 0; return; }

    for (size_t i = 0; i < sz; ++i) is_comp[i] = false;
    is_comp[0] = is_comp[1] = true;

    uint64_t sq = (uint64_t)sqrt((double)limit);
    for (uint64_t i = 2; i <= sq; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;
        }
    }

    /* Count primes to allocate exact size */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++cnt;

    uint64_t *primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!primes) { free(is_comp); *out = NULL; *out_cnt = 0; return; }

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) primes[idx++] = i;

    free(is_comp);
    *out = primes;
    *out_cnt = cnt;
}

/* ------------------------------------------------------------------ */
/* Compute the bound for the nth prime (n >= 1).                        */
static uint64_t prime_upper_bound(idx_t n)
{
    if (n < 6) return 15ULL;
    double nd = (double)n;
    return (uint64_t)(nd * (log(nd) + log(log(nd))) + 3.0);
}

/* ------------------------------------------------------------------ */
/* Rebuild the cache of primes and f(p) values up to prime index
 * `up_to_index` (inclusive).                                           */
static void rebuild_cache_up_to(idx_t up_to_index)
{
    if (cached_prime_cnt >= (size_t)up_to_index) return;  /* already ok */

    /* Free old data */
    free(cached_primes);
    free(cached_f);
    cached_primes = NULL;
    cached_f = NULL;
    cached_prime_cnt = 0;

    /* Calculate the sieve limit needed for up_to_index primes */
    uint64_t limit = prime_upper_bound(up_to_index);

    /* Build primes */
    uint64_t *primes = NULL;
    size_t cnt = 0;
    sieve_primes_upto(limit, &primes, &cnt);
    if (!primes) return;   /* allocation failure */

    cached_primes = primes;
    cached_prime_cnt = cnt;

    /* Allocate array for f(p) values */
    seq_t *f = (seq_t*)malloc(cnt * sizeof(seq_t));
    if (!f) return;  /* allocation failure */

    /* Compute f(p) for each prime */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = cached_primes[i];
        uint64_t best = 1ULL;  /* 1 always qualifies */
        if (p > 2ULL) {
            for (size_t j = 0; j < cnt; ++j) {
                uint64_t q = cached_primes[j];
                uint64_t q2 = q * q;
                if (q2 >= p) break;
                uint64_t cand = ((p - 1ULL) / q2) * q2;
                if (cand > best) best = cand;
            }
        }
        f[i] = (seq_t)best;
    }

    cached_f = f;
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                           */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, return 0 for safety */

    /* Ensure we have primes up to prime(n+1) */
    rebuild_cache_up_to(n + 1ULL);

    /* Compute the first difference */
    seq_t v1 = cached_f[n];      /* f(p_{n+1})  */
    seq_t v0 = cached_f[n - 1];  /* f(p_{n})    */
    return (seq_t)(v1 - v0);
}
