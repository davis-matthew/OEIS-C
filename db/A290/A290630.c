/*
 * A290630  a(n+1) = a(n) + (final digit of greatest prime < a(n)); a(1)=3.
 * https://oeis.org/A290630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9200 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3, a(n+1)=a(n)+(last digit of greatest prime < a(n)) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Cache that stores the computed terms of the sequence. */
static seq_t *cache = NULL;        /* array of terms */
static size_t cache_len = 0;       /* number of terms currently stored */
static size_t cache_cap = 0;       /* allocated capacity */

/* Return the greatest prime < x (x >= 3). */
static seq_t greatest_prime_lt(seq_t x)
{
    seq_t p = x - (seq_t)1;            /* start at the largest number < x */
    while (!is_prime_big(p)) {         /* search downward */
        p = p - (seq_t)1;
    }
    return p;
}

/* a(n) for n >= 1 (a(1)=3).  Returns 0 for n == 0. */
seq_t a(idx_t n)
{
    if (n == 0)                       /* out of bounds */
        return (seq_t)0;

    /* Grow the cache if needed. */
    if (cache_cap < (size_t)n) {
        seq_t *tmp = seq_realloc(cache, cache_cap, (size_t)n);
        cache = tmp;
        cache_cap = (size_t)n;
        if (cache == NULL)            /* allocation failed */
            return (seq_t)0;
    }

    /* Initialise the first value if the cache is empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)3;
        cache_len = 1;
    }

    /* Compute any missing terms up to and including a(n). */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t gp   = greatest_prime_lt(prev);
        seq_t d    = gp % (seq_t)10;   /* last decimal digit of gp */
        cache[i]   = prev + d;
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
