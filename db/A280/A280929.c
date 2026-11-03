/*
 * A280929  Number of steps required to reach the first prime when starting
 *          from n in the Collatz (or '3x+1') problem.
 * https://oeis.org/A280929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 9262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9263 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of Collatz steps required to reach the first prime
   encountered when starting from n.  The first step is always taken,
   so n being prime does not terminate immediately.  Example: a(33)=6. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *cache = NULL;    /* cache[n] = a(n) or 0 if not yet computed */
static size_t cache_cap = 0;   /* allocated capacity of the cache */

/* Ensure that the cache array can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* indices 0..n */
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) {                /* allocation failed – no caching */
        mem_free(cache);
        cache = NULL;
        cache_cap = 0;
        return;
    }
    cache = tmp;
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = 0;
    cache_cap = new_cap;
}

/* Compute a(n).  Returns 0 for n == 0 (unused by OEIS). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Try to use a cached value first.  Cache has been grown if necessary. */
    ensure_cache(n);
    size_t idx = (size_t)n;
    if (cache && cache[idx] != 0)
        return cache[idx];

    seq_t steps = 0;
    seq_t cur = (seq_t)n;

    while (true) {
        /* Perform one Collatz step. */
        if (cur & 1)          /* odd */
            cur = cur * 3 + 1;
        else                  /* even */
            cur >>= 1;

        ++steps;

        /* Check if the new value is prime. */
        if (is_prime_big(cur)) {
            /* Store in cache before returning. */
            if (cache)
                cache[idx] = steps;
            return steps;
        }
    }
}
