/*
 * A271564  Number of 6's found in the first differences of a reduced residue
 *          system modulo a primorial p#.
 * https://oeis.org/A271564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*∏(p-2) – 2*∏(p-3), product over primes p ≤ pₙ, p>3
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (where pₙ is the nth prime; a(1)=0, a(2)=0) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- cumulative state --------------------------------------------------- */
static size_t primes_cap = 0;          /* allocated size of primes[]   */
static uint32_t *primes = NULL;        /* list of primes, ascending    */
static size_t primes_cnt = 0;          /* number of primes stored      */

static size_t cache_cap = 0;           /* allocated size of a_cache[]  */
static seq_t *a_cache = NULL;          /* cached values a(1)...a(n)    */
static idx_t cached_n = 0;             /* highest index computed       */

static seq_t prod2 = (seq_t)1;         /* ∏(p-2) for primes with p>3   */
static seq_t prod3 = (seq_t)1;         /* ∏(p-3) for primes with p>3   */

/* ---- prime generation --------------------------------------------------- */
static void ensure_primes_cap(size_t need) {
    if (primes_cap >= need) return;
    size_t new_cap = primes_cap == 0 ? 8 : primes_cap * 2;
    if (new_cap < need) new_cap = need;
    uint32_t *new_arr = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    primes = new_arr;
    primes_cap = new_cap;
}

static void add_next_prime(void) {
    /* trial‑division generation.  We keep primes in increasing order. */
    uint32_t candidate = primes_cnt ? primes[primes_cnt - 1] + 2 : 3;
    while (true) {
        bool ok = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > candidate) break;
            if (candidate % p == 0) { ok = false; break; }
        }
        if (ok) break;
        candidate += 2;
    }
    ensure_primes_cap(primes_cnt + 1);
    primes[primes_cnt++] = candidate;
}

static void ensure_primes_upto_idx(idx_t n) {
    if (primes_cnt < (size_t)n) {
        if (primes_cnt == 0) {
            /* seed with first two primes 2 and 3 */
            ensure_primes_cap(2);
            primes[0] = 2;
            primes[1] = 3;
            primes_cnt = 2;
        }
        while (primes_cnt < (size_t)n)
            add_next_prime();
    }
}

/* ---- cache handling ----------------------------------------------------- */
static void grow_cache(size_t need) {
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = new_arr;
    cache_cap = new_cap;
}

/* ---- main function ------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* undefined, but safe */

    /* make sure primes and cache are ready up to n */
    ensure_primes_upto_idx(n);
    grow_cache((size_t)n);

    /* compute values for indices > cached_n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        uint32_t p = primes[i - 1];
        if (p >= 5) {
            prod2 = prod2 * (seq_t)(p - 2);
            prod3 = prod3 * (seq_t)(p - 3);
        }
        seq_t val = (seq_t)0;
        if (i > 2) {
            seq_t t1 = prod2 * (seq_t)2;
            seq_t t2 = prod3 * (seq_t)2;
            val = t1 - t2;
        }
        a_cache[i - 1] = val;
    }
    cached_n = n;
    return a_cache[n - 1];
}
