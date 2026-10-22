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
 * Helpers:   append_candidate, digits_of, isPrimeGeneric, next_prime, pow10
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

#include "../utils/next_prime.h"
#include "../utils/append_candidate.h"
#include "../utils/digits_of.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/pow10.h"

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
    *ptr = (seq_t*)realloc(*ptr, new_cap * sizeof(seq_t));
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
        unsigned int len = digits_of(p);            /* number of decimal digits of p */
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
