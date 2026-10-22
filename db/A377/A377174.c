/*
 * A377174  Primes p such that 5/2 is a primitive root modulo p.
 * https://oeis.org/A377174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   factor_u64, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A377174: primes p such that 5/2 is a primitive root modulo p */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/factor_u64.h"

static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap * 2);
        while (new_cap < needed) new_cap <<= 1;
        *ptr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Test whether 5/2 is a primitive root modulo the odd prime p.
   Reason: g = 5 * inv2 mod p; g is a primitive root iff
   g^( (p-1)/q ) != 1 for every distinct prime divisor q of p-1. */
static bool is_primitive_root_5over2(uint64_t p)
{
    if ((p & 1u) == 0) return false;          /* skip even primes */

    seq_t g = ((seq_t)5 * ((p + 1) / 2)) % (seq_t)p;
    if (g == 0 || g == 1) return false;

    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_u64(p - 1, primes, exps, &pcnt);

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t q   = primes[i];
        uint64_t exp = (p - 1) / q;
        if (mod_pow(g, exp, (seq_t)p) == (seq_t)1) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t last_checked = 0;     /* prime just tested */

    if (n == 0) return 0;   /* offset is 1 */

    /* grow cache up to required size */
    ensure_capacity((size_t)n, &cache_cap, &cache);

    /* advance the search from where we left off */
    while (cache_len < (size_t)n) {
        last_checked = (last_checked == 0) ? 2 : next_prime(last_checked);
        if (is_primitive_root_5over2(last_checked))
            cache[cache_len++] = (seq_t)last_checked;
    }
    return cache[n - 1];
}
