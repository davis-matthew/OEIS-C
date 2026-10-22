/*
 * A352476  Expansion of g.f.: 1/Sum_{p prime} x^p.
 * https://oeis.org/A352476
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  sign
 * Formula:   a(-2) = 1; a(n) = -Σ_{k=1..n+2} [is_prime(k+2)] * a(n-k) (where
 *            [is_prime(m)] is 1 if m is prime, else 0)
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime sieve helper ---------- */
static bool *prime_cache      = NULL; /* prime_cache[i] == true ⇔ i is prime */
static size_t prime_cache_sz = 0;    /* number of indices stored (0..sz-1) */

static void compute_primes(size_t limit)
{
    if (prime_cache_sz > limit) return;          /* already large enough */

    bool *old = prime_cache;
    size_t new_sz = limit + 1;                   /* include index limit */
    prime_cache = (bool*)calloc(new_sz, sizeof(bool));
    if (!prime_cache) exit(1);

    /* mark primes using simple sieve of Eratosthenes */
    for (size_t i = 2; i <= limit; ++i) prime_cache[i] = true;
    for (size_t p = 2; p * p <= limit; ++p) if (prime_cache[p]) {
        for (size_t q = p * p; q <= limit; q += p) prime_cache[q] = false;
    }

    prime_cache_sz = new_sz;
    free(old);
}

/* ---------- Sequence cache helper ---------- */
static seq_t *seq_cache      = NULL; /* stores a(n) offset by +2: a(n) → seq_cache[n+2] */
static size_t seq_cache_sz   = 0;    /* number of elements currently stored */

static void ensure_cached(idx_t n)
{
    if (n < -2) return;                 /* undefined, nothing to do */

    size_t target_idx = (size_t)(n + 2);
    if (seq_cache_sz > target_idx) return; /* already cached */

    size_t old_sz = seq_cache_sz;
    size_t new_sz = target_idx + 1;           /* include target_idx */

    seq_t *tmp = (seq_t*)realloc(seq_cache, new_sz * sizeof(seq_t));
    if (!tmp && new_sz > 0) exit(1);
    seq_cache = tmp;
    seq_cache_sz = new_sz;

    /* first term (a(-2) = 1) if this is the first allocation */
    if (old_sz == 0) {
        seq_cache[0] = 1;                      /* a(-2) = 1 */
        old_sz = 1;
    }

    /* compute missing terms */
    for (size_t i = old_sz; i < new_sz; ++i) {
        idx_t cur_n = (idx_t)i - 2;          /* current n */

        /* ensure primes up to cur_n+4 for k+2 check */
        compute_primes((size_t)(cur_n + 4));

        seq_t sum = 0;
        idx_t kmax = cur_n + 2;
        for (idx_t k = 1; k <= kmax; ++k) {
            idx_t p = k + 2;                 /* number to check for primality */
            if (prime_cache[p]) {
                size_t sub_idx = (size_t)(cur_n - k + 2);
                sum += seq_cache[sub_idx];
            }
        }
        seq_cache[i] = -sum;                 /* a(n) = -sum */
    }
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    if (n < -2) return 0;          /* undefined for n < -2 */
    ensure_cached(n);
    return seq_cache[(size_t)(n + 2)];
}
