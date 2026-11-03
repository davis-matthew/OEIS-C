/*
 * A360781  Primes p such that at least one number remains prime when p is
 *          bracketed by a single digit d; that is, at least one instance of
 *          d//p//d is prime where // means concatenation.
 * https://oeis.org/A360781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9248 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, next_prime, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A360781: primes p such that for some single digit d∈{1,3,7,9}
   the concatenation d‖p‖d is prime.  The function builds the sequence
   lazily, storing discovered terms in a static cache for fast repeated
   access.  No hard‑coded values are used.  The implementation follows
   the definition exactly.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
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

/* static cache of found terms */
static seq_t *cache      = NULL;  /* array of seq_t */
static size_t cache_len  = 0;     /* number of valid terms in cache */
static size_t cache_cap  = 0;     /* allocated capacity of cache */

/* last prime examined; starts at 0 so that next_prime(0) returns 2. */
static uint64_t last_prime = 0;

/* helper: grow cache as needed */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (needed <= *cap)
        return;
    size_t new_cap = *cap ? (*cap << 1) : 64;
    if (new_cap < needed)
        new_cap = needed;
    *ptr = mem_resize(seq_t, *ptr, new_cap);
    *cap = new_cap;
}

/* extend cache until it contains at least n terms (n≥1) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;                     /* already enough terms */

    while (cache_len < (size_t)n) {
        /* get next prime after the last one found */
        uint64_t p = next_prime(last_prime);
        last_prime = p;

        /* test the four allowed digits */
        bool ok = false;
        const unsigned int ds[4] = {1, 3, 7, 9};
        unsigned int len = num_digits_idx(p);            /* number of decimal digits of p */
        seq_t pow = pow10((idx_t)(len + 1));         /* 10^(len+1) */

        for (int i = 0; i < 4 && !ok; ++i) {
            seq_t d   = (seq_t)ds[i];
            seq_t num = d * pow + (seq_t)p * 10 + d;  /* d‖p‖d */
            if (isPrimeGeneric(num))
                ok = true;
        }

        if (ok) {
            ensure_capacity(cache_len + 1, &cache_cap, &cache);
            cache[cache_len++] = (seq_t)p;
        }
    }
}

/* public sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n<1; return 0 for safety */

    ensure_cache(n);
    return cache[n - 1];
}
