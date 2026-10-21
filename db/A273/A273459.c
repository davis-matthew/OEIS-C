/*
 * A273459  Even numbers such that the sum of the odd divisors is a prime p
 *          and the sum of the even divisors is 2p.
 * https://oeis.org/A273459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * q^2, where q is odd and sigma(q^2) is prime (OEIS
 *            A273459)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 394, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches for the n-th such q, stores 2*q^2 in a cache, and returns it. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/factor_u64.h"

static seq_t *cache = NULL;      /* cache of sequence terms */
static size_t cache_cap = 0;     /* current allocated capacity */
static size_t cache_len = 0;     /* number of terms cached */
static uint64_t last_checked = 1;   /* last odd number examined */

/* Ensure the dynamic array has room for at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return; /* silently abort on OOM; the caller checks cache_len */
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Test whether sigma(q^2) is prime. q is odd and fits in uint64_t. */
static bool sigma_square_is_prime(uint64_t q)
{
    /* Factor q */
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_u64(q, primes, exps, &pcnt);

    /* Compute sigma(q^2) = Π(1 + p + ... + p^{2a}) */
    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        unsigned int a = (unsigned int)exps[i];
        seq_t term = (seq_t)1;      /* p^0 */
        seq_t sum  = (seq_t)0;      /* 1 + p + ... + p^{2a} */
        for (unsigned int k = 0; k <= 2u * a; ++k) {
            sum += term;
            term *= (seq_t)p;
        }
        sigma *= sum;
    }
    return isPrimeGeneric(sigma);
}

/* Extend the cache until it contains at least 'target' terms. */
static void build_until(idx_t target)
{
    while ((idx_t)cache_len < target) {
        /* Find next odd q with sigma(q^2) prime */
        uint64_t q = last_checked + 2;  /* start from the next odd */
        while (!sigma_square_is_prime(q)) {
            q += 2;
        }
        last_checked = q;

        /* a = 2 * q^2 */
        seq_t val = ((seq_t)2) * ((seq_t)q) * ((seq_t)q);
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = val;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    build_until(n);
    return cache[n - 1];
}
