/*
 * A370575  Least increasing sequence of semiprimes with alternating parity
 *          such that a(n) - a(n-1) is a semiprime, with a(1) = 4.
 * https://oeis.org/A370575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 2216, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A370575:
   a(1) = 4.
   For n > 1, a(n) is the smallest semiprime with parity alternating to a(n-1)
   such that a(n) – a(n-1) is also a semiprime. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* --- Small primality test for 64‑bit values -------------------------------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* --- Semiprime test on 64‑bit unsigned ------------------------------------- */
static bool is_semiprime_uint64(uint64_t n)
{
    if (n < 4) return false;             /* 2*2 is the smallest semiprime */
    uint64_t p = 0;
    /* find smallest factor */
    for (p = 2; p * p <= n; ++p) {
        if (n % p == 0) break;
    }
    if (p * p > n) return false;         /* n is prime */
    uint64_t q = n / p;
    if (q == p) return true;             /* square of a prime */
    return is_prime_uint64(q);           /* p is prime, q must be prime */
}

/* --- Compute next term given previous value ------------------------------- */
static seq_t next_term(seq_t prev)
{
    uint64_t prev64 = (uint64_t)prev;
    bool need_odd = (prev64 % 2 == 0);   /* even previous → need odd next */
    uint64_t cand = prev64 + 1;

    for (;; ++cand) {
        /* enforce parity alternation */
        if (need_odd && (cand & 1ULL) == 0) continue;
        if (!need_odd && (cand & 1ULL) == 1) continue;

        if (!is_semiprime_uint64(cand)) continue;

        uint64_t diff = cand - prev64;
        if (is_semiprime_uint64(diff)) return (seq_t)cand;
    }
}

/* --- Cache to support out‑of‑order queries -------------------------------- */
static seq_t *seq_cache = NULL;
static idx_t   cache_len = 0;     /* number of terms stored (a(1) at index 0) */
static size_t  cache_cap = 0;     /* allocated capacity */

static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;

    /* Grow the array if necessary */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first term if this is the first request */
    if (cache_len == 0) {
        seq_cache[0] = (seq_t)4;   /* a(1) */
        cache_len = 1;
    }

    /* Fill missing terms up to n */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_cache[i] = next_term(seq_cache[i - 1]);
    }
    cache_len = n;
}

/* --- Public API ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* defined only for n >= 1 */
    ensure_cache(n);
    return seq_cache[n - 1];
}
