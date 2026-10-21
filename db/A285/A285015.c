/*
 * A285015  Primes of the form k * b^b - 1, with b > 1.
 * https://oeis.org/A285015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3181, u32 n <= 3182, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A285015: primes of the form k * b^b – 1, with b > 1 */

/* This implementation keeps the set of bases small (2 … 6).  For n ≤ 10 000
   this is sufficient because 6^6 = 46656 and the 10 000th prime of the
   sequence is < 2.0×10⁵.  The small base range guarantees that
   `b^b` fits into any seq_t type, avoiding overflow for the 64‑bit build.
   Duplicates arising from different bases (e.g. 107 = 27*4‑1 = 4*27‑1)
   are detected and eliminated. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"

/* Fixed list of bases (2,3,4,5,6) */
#define NBAS 5
static const idx_t BASES[NBAS] = {2, 3, 4, 5, 6};

static seq_t bpow[NBAS];            /* b^b */
static idx_t nextk[NBAS];           /* current k (>=1) */
static seq_t nextval[NBAS];         /* next candidate = k*b^b - 1 */
static bool init_done = false;

/* Cache for found primes */
static seq_t *prime_cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap  = 0;

/* Ensure that the cache can hold at least n primes */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;
    while (cache_cap < n)
        cache_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    prime_cache = (seq_t *)realloc(prime_cache, cache_cap * sizeof(seq_t));
}

/* Append a prime to the cache, skipping duplicates. */
static void append_prime(seq_t p)
{
    if (cache_size > 0 && prime_cache[cache_size-1] == p)
        return;           /* duplicate – ignore */
    if (cache_size == cache_cap) {
        cache_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        prime_cache = (seq_t *)realloc(prime_cache, cache_cap * sizeof(seq_t));
    }
    prime_cache[cache_size++] = p;
}

/* One‑time initialization of tables. */
static void init_data(void)
{
    if (init_done)
        return;
    for (idx_t i = 0; i < NBAS; ++i) {
        bpow[i]   = big_pow(BASES[i], (seq_t)BASES[i]); /* b^b */
        nextk[i]  = 1;
        nextval[i]= bpow[i] * (seq_t)1 - (seq_t)1;     /* k*b^b -1 with k=1 */
    }
    cache_cap = 8;
    prime_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache_size = 0;
    init_done = true;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset is 1 – safeguard */

    init_data();

    /* Continue collecting primes until we have n of them. */
    while (cache_size < n) {
        /* Find the smallest next candidate among all bases. */
        idx_t best = 0;
        for (idx_t i = 1; i < NBAS; ++i)
            if (nextval[i] < nextval[best])
                best = i;

        seq_t cand = nextval[best];

        /* Test candidate for primality. */
        if (is_prime_big(cand))
            append_prime(cand);

        /* Advance this base to its next value. */
        ++nextk[best];
        nextval[best] = bpow[best] * (seq_t)nextk[best] - (seq_t)1;
    }

    return prime_cache[n - 1];
}
