/*
 * A323617  Primes p such that 3 is a primitive root modulo p while 243 is
 *          not.
 * https://oeis.org/A323617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 520, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9681 (2026-11-01)
 * Helpers:   factorize, is_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code searches for primes  p ≡ 1 (mod 5) that have 3 as a primitive root
 * and 243 not as a primitive root.  a(n) is the nth such prime (a(1)=31). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factorize.h"
#include "../utils/is_prime.h"
#include "../utils/powmod.h"

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

static seq_t *cache = NULL;      /* array of found primes */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static size_t cache_len = 0;     /* number of primes stored */
static uint64_t next_candidate = 2; /* integer to test next */

/* Append a prime to the cache, enlarging the allocation if necessary */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Build the cache up to at least n terms */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* find next prime */
        do {
            if (++next_candidate < 2)
                next_candidate = 2;
        } while (!is_prime(next_candidate));

        /* only consider primes ≡ 1 (mod 5) */
        if ((next_candidate % 5) != 1)
            continue;

        /* factorize p-1 */
        uint64_t primes[64];
        int exps[64];
        int pf_cnt = factorize(next_candidate - 1, primes, exps);

        /* check that 3 is a primitive root modulo p */
        bool three_primitive = true;
        for (int i = 0; i < pf_cnt && three_primitive; ++i) {
            seq_t pow = powmod((seq_t)3,
                                (next_candidate - 1) / primes[i],
                                (seq_t)next_candidate);
            if (pow == (seq_t)1)
                three_primitive = false;
        }
        if (!three_primitive)
            continue;

        /* ensure 243 is NOT a primitive root modulo p */
        bool low_order = false;
        for (int i = 0; i < pf_cnt && !low_order; ++i) {
            seq_t pow = powmod((seq_t)243,
                                (next_candidate - 1) / primes[i],
                                (seq_t)next_candidate);
            if (pow == (seq_t)1)
                low_order = true;
        }
        if (!low_order)
            continue; /* 243 would be a primitive root – skip */

        /* all conditions satisfied; store the prime */
        cache_append((seq_t)next_candidate);
    }
}

/* Required sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n - 1];
}
