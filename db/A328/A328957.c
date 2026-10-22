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
 * Helpers:   count_distinct_primes, count_prime_factors, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers k such that σ₀(k) != ω(k) * Ω(k)
   (OEIS A328957).  On the first call the function checks integers
   starting from 1 and appends those satisfying the inequality to a
   static cache until it has found the requested n‑th element. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/count_prime_factors.h"

/* Append a value to the dynamic cache.  The cast is needed for C++ */
static void append_cache(seq_t **cache, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *new_cache = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
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
