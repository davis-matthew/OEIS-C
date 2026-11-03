/*
 * A285694  a(1) = 2; a(n + 1) = smallest prime > a(n) such that a(n + 1) -
 *          a(n) is the product of 10 primes.
 * https://oeis.org/A285694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-03)
 * Helpers:   bigomega_seq, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(1) = 2;  a(n+1) = smallest prime > a(n) such that
   a(n+1) - a(n) has exactly 10 prime factors (counted with multiplicity). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/bigomega_seq.h"
#include "../utils/next_prime.h"
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Grow cache to at least 'needed' elements. */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (*cap < needed) {
        size_t newcap = (*cap == 0) ? 4 : *cap * 2;
        while (newcap < needed) newcap *= 2;
        seq_t *tmp;
        if (*ptr == NULL) {
            tmp = seq_alloc(newcap);
        } else {
            tmp = seq_realloc(*ptr, *cap, newcap);
        }
        *ptr = tmp;
        *cap = newcap;
    }
}

seq_t a(idx_t n)
{
    /* Static cache of previously generated terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;   /* number of terms stored, cache[0] = a(1) */

    /* Initialise cache with first term. */
    if (cache_size == 0) {
        ensure_capacity(1, &cache_cap, &cache);
        cache[0] = (seq_t)2;     /* a(1) = 2 */
        cache_size = 1;
    }

    /* Generate terms up to requested index. */
    while (cache_size < n) {
        seq_t last = cache[cache_size - 1];

        /* Find the next prime > last. */
        uint64_t start = (uint64_t)last + 1;
        uint64_t p = next_prime(start);
        seq_t candidate = (seq_t)p;

        /* Loop until we find a prime whose distance from last has 10
           prime factors (with multiplicity). */
        while (true) {
            seq_t diff = candidate - last;
            if (bigomega_seq(diff) == (seq_t)10) {
                break;
            }
            /* Advance to the next prime. */
            uint64_t np = (uint64_t)candidate + 1;
            p = next_prime(np);
            candidate = (seq_t)p;
        }

        /* Append candidate to cache. */
        ensure_capacity(cache_size + 1, &cache_cap, &cache);
        cache[cache_size] = candidate;
        ++cache_size;
    }

    return cache[n - 1];
}
