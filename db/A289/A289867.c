/*
 * A289867  Primes obtained from other primes by prefixing a 6.
 * https://oeis.org/A289867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 241, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9783 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A289867 – primes obtained by prefixing a 6 to a prime.
   The n-th term is the n-th smallest prime of the form 6·10^d + q,
   where q is a d‑digit prime and the result is also prime.
   The function builds a cache incrementally, so repeated calls with
   arbitrary order of n are supported. */

#include <stddef.h>
#include <stdbool.h>

#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/num_digits0.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

/* Return the n‑th term of A289867 (1‑based).  For n == 0 the value is
   undefined; the function returns 0 to avoid a crash. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Static cache of found terms */
    static seq_t *cache   = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;

    /* Current suffix prime to test */
    static uint64_t next_suffix = 2;     /* first prime to examine */

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = 16;                  /* initial capacity */
        cache = seq_alloc(cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t q = next_suffix;        /* current suffix prime */

        /* Number of decimal digits of q (q is 64‑bit, so digit_count works) */
        unsigned int d = (unsigned int)num_digits0(q);

        /* 10^d as a seq_t value */
        seq_t pow10_val = pow10((idx_t)d);

        /* Candidate number: 6·10^d + q */
        seq_t candidate = (seq_t)6 * pow10_val + (seq_t)q;

        /* Keep it if the candidate is prime */
        if (is_prime_big(candidate)) {
            if (cache_len == cache_cap) {          /* grow cache */
                size_t new_cap = cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }

        /* Advance to the next suffix prime */
        next_suffix = next_prime(q + 1);
    }

    /* Return the n‑th term (1‑based indexing) */
    return cache[n - 1];
}
