/*
 * A245209  Nonprimes such that it takes exactly 4 iterations of
 *          reverse-and-add digits to generate a prime.
 * https://oeis.org/A245209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-03)
 * Helpers:   is_prime_u64, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A245209 – Nonprime numbers that take exactly four
   reverse‑and‑add iterations to become a prime.  The function
   a(n) returns the n‑th such number (offset 0).  It searches
   candidates incrementally and memoises the results.  The
   implementation relies on the supplied arbitrary‑precision helper
   functions for memory management. */

#include <stdlib.h>
#include <stdbool.h>

/* Utility functions for prime testing and digit reversal */
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/reverse_digits.h"
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free(seq_t *p) { mem_free(p); }

/* Utility functions for dynamic array management of seq_t */

/* static cache of sequence terms */
static seq_t *cache = NULL;      /* array of discovered terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of terms stored */
static uint64_t next_candidate = 1; /* candidate to test next */

/* Append a newly found term to the cache, reallocating if necessary. */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        if (cache)
            seq_free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Ensure that the cache contains at least (n+1) terms. */
static void ensure_terms(idx_t n)
{
    while (cache_len <= (size_t)n) {
        seq_t x = (seq_t)next_candidate;
        ++next_candidate;

        /* Must start as a non‑prime. */
        if (is_prime_big(x))
            continue;

        seq_t y = x;
        bool fail = false;

        /* Perform up to four reverse‑and‑add steps. */
        for (int step = 1; step <= 4; ++step) {
            y = reverse_digits(y) + y;      /* reverse‑and‑add */

            /* If an intermediate number is prime, this candidate fails. */
            if (step < 4 && is_prime_big(y)) {
                fail = true;
                break;
            }
        }

        /* Fourth step must yield a prime, others must not. */
        if (fail || !is_prime_big(y))
            continue;

        /* Record the term. */
        append_cache(x);
    }
}

/* Public API required by the test harness */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return cache[(size_t)n];
}
