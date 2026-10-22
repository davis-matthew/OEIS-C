/*
 * A369939  Numbers whose maximal exponent in their prime factorization is a
 *          Fibonacci number.
 * https://oeis.org/A369939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th integer whose maximal exponent in its prime
 *            factorization is a Fibonacci number
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For the implementation we keep an incremental cache of the terms and
   test integers in increasing order.  The primality test is
   performed via the provided factor_u64 helper.  This works for all
   supported seq_t sizes. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Cache of generated terms */
static seq_t *cache = NULL;      /* cached terms (in order) */
static size_t cache_size = 0;    /* number of terms already cached */
static size_t cache_cap  = 0;    /* capacity of the cache array */

/* The next integer to test for inclusion in the sequence */
static uint64_t next_candidate = 1;

/* Fibonacci numbers up to 55 (the largest <= 64, the maximum possible exponent for 64‑bit integers) */
static const int fibs[] = {1, 2, 3, 5, 8, 13, 21, 34, 55};
static const size_t fib_count = sizeof(fibs) / sizeof(fibs[0]);

/* Return true if e is a Fibonacci number (from the table above). */
static bool is_fib_exp(int e)
{
    if (e <= 0) return false;
    for (size_t i = 0; i < fib_count; ++i) {
        if (e == fibs[i]) return true;
        if (e < fibs[i]) break;
    }
    return false;
}

/* Determine whether integer n satisfies the sequence condition. */
static bool good_num(uint64_t n)
{
    if (n == 1) return true;          /* 1 is a term */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    int max_exp = 0;
    for (size_t i = 0; i < cnt; ++i) {
        int e = (int)exps[i];
        if (e > max_exp) max_exp = e;
        /* early exit if max_exp already exceeds the largest fib in the table */
        if (max_exp > fibs[fib_count - 1]) return false;
    }
    return is_fib_exp(max_exp);
}

/* Grow the cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)n) return;

    /* Allocate or enlarge the cache array. */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_size < (size_t)n) {
        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            seq_t *tmp_ptr = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp_ptr) return;   /* allocation failed; abort caching */
            cache = tmp_ptr;
        }
        if (good_num(next_candidate)) {
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n - 1];
}
