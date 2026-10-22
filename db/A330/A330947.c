/*
 * A330947  Nonprime numbers whose prime indices are all prime numbers.
 * https://oeis.org/A330947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th nonprime number such that each prime factor's *
 *            index in the sequence of primes is itself a prime. This defines
 *            OEIS * A330947
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   compute_pi, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime.h"    /* bool is_prime(idx_t) */
#include "../utils/compute_pi.h"  /* seq_t compute_pi(idx_t) */

static seq_t *seq_cache = NULL;  /* cached sequence values */
static size_t seq_len   = 0;     /* current number of cached values */
static size_t seq_cap   = 0;     /* capacity of seq_cache */

/* Prime sieve up to PRIME_LIMIT for factorisation */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static const uint64_t PRIME_LIMIT = 1000000ULL;   /* sufficient for ~10 000 terms */

static uint64_t next_candidate = 2;   /* next integer to test for inclusion */

/* ------------------------------------------------------------------ */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_cache == NULL || seq_cap < needed) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap   = new_cap;
    }
}

/* ------------------------------------------------------------------ */
static void init_prime_list(void)
{
    if (prime_list != NULL) return;

    uint64_t limit = PRIME_LIMIT;
    bool *sieve = (bool *)calloc(limit + 1, sizeof(bool));
    if (!sieve) exit(1);   /* allocation failure */
    sieve[0] = sieve[1] = true;
    for (uint64_t i = 2; i * i <= limit; ++i)
        if (!sieve[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                sieve[j] = true;

    /* Count primes */
    prime_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!sieve[i]) ++prime_cnt;

    prime_list = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!sieve[i]) prime_list[idx++] = i;

    free(sieve);
}

/* ------------------------------------------------------------------ */
static size_t prime_index(uint64_t p)
{
    /* Binary search in the pre‑computed prime list */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t mid_val = prime_list[mid];
        if (mid_val == p)          return mid + 1; /* 1‑based index */
        if (mid_val < p)           lo = mid + 1;
        else                       hi = mid;
    }
    /* p is larger than our pre‑computed limit */
    seq_t pi = compute_pi(p);
    return (size_t)pi;
}

/* ------------------------------------------------------------------ */
static void build_until(idx_t n)
{
    init_prime_list();

    /* a(1) = 1 by definition */
    if (seq_len == 0) {
        ensure_seq_capacity(16);
        seq_cache[0] = (seq_t)1;
        seq_len = 1;
    }

    while (seq_len < n) {
        uint64_t cand = next_candidate++;
        if (is_prime((idx_t)cand)) continue;   /* primes are excluded */

        uint64_t temp = cand;
        bool ok = true;

        /* Trial division with primes up to sqrt(temp) */
        for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
            uint64_t p = prime_list[i];
            if (temp % p == 0) {
                size_t idx = i + 1;                /* index of p (1‑based) */
                if (!is_prime((idx_t)idx)) { ok = false; break; }
                while (temp % p == 0) temp /= p;   /* remove multiplicity */
            }
        }
        if (!ok) continue;

        /* Remaining factor greater than sqrt(temp) is prime, if any */
        if (temp > 1) {
            size_t idx = prime_index(temp);
            if (!is_prime((idx_t)idx)) continue;
        }

        /* cand satisfies the condition; store it */
        ensure_seq_capacity(seq_len + 1);
        seq_cache[seq_len++] = (seq_t)cand;
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    build_until(n);
    return seq_cache[n - 1];
}
