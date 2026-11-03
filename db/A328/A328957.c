/*
 * A328957  Numbers k such that sigma_0(k) != omega(k) * Omega(k), where
 *          sigma_0 = A000005, omega = A001221, Omega = A001222.
 * https://oeis.org/A328957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9241 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers k such that σ₀(k) != ω(k) * Ω(k)
   (OEIS A328957).  On the first call the function checks integers
   starting from 1 and appends those satisfying the inequality to a
   static cache until it has found the requested n‑th element. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Count divisors of a 64‑bit integer */
static uint64_t divisor_count_u64(uint64_t n)
{
    uint64_t tau = 1;
    uint64_t exp;

    /* factor 2 */
    exp = 0;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        ++exp;
    }
    if (exp) tau *= (exp + 1);

    /* factor odd primes */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }

    /* remaining prime factor */
    if (n > 1) tau *= 2;

    return tau;
}

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

/* Count prime factors of m (with multiplicity) */
static seq_t count_prime_factors(seq_t m) {
    seq_t cnt = 0;
    for (idx_t p = 2; (idx_t)p * (idx_t)p <= m; ++p) {
        while (m % p == 0) {
            ++cnt;
            m /= p;
        }
    }
    if (m > 1) ++cnt;
    return cnt;
}

/* Append a value to the dynamic cache.  The cast is needed for C++ */
static void append_cache(seq_t **cache, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *new_cache = mem_resize(seq_t, *cache, new_cap);
        *cache = new_cache;
        *cap   = new_cap;
    }
    (*cache)[(*len)++] = val;
}

/* Test whether a number satisfies the condition σ₀(k) ≠ ω(k)·Ω(k) */
static bool is_included(idx_t num)
{
    uint64_t n = (uint64_t)num;

    uint64_t sigma   = divisor_count_u64(n);          /* σ₀(n) */
    int      distinct = count_distinct_primes(n);     /* ω(n)  */
    seq_t    omega    = count_prime_factors((seq_t)n);/* Ω(n)  */

    seq_t prod = omega * (seq_t)distinct;
    return ((seq_t)sigma) != prod;
}

/* Static cache for the sequence */
static seq_t *cache       = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;
static idx_t last_checked = 0;

/* OEIS sequence A328957: a(n) = the nth number k with σ₀(k) ≠ ω(k)*Ω(k) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* undefined for n = 0 */

    while (cache_len < n) {
        ++last_checked;                    /* next candidate integer */
        if (is_included(last_checked))
            append_cache(&cache, &cache_len, &cache_cap, (seq_t)last_checked);
    }
    return cache[n - 1];
}
