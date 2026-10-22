/*
 * A302568  Odd numbers that are either prime or whose prime indices are
 *          pairwise coprime.
 * https://oeis.org/A302568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd numbers that are either prime or whose prime indices
 *            are pairwise coprime
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   factor_u64, gcd64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/factor_u64.h"
#include "../utils/gcd64.h"

/* small gcd for 64‑bit values */

/* General dynamic array grow helper for seq_t */
static inline void grow_seq(void **p, size_t *cap, size_t need) {
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 8 : (*cap << 1);
        while (newcap < need) newcap <<= 1;
        *p = realloc(*p, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* --------- static state --------- */
static seq_t *cache = NULL;      /* computed sequence values   */
static size_t cache_len = 0;
static size_t cache_cap = 0;

static seq_t *primes = NULL;     /* list of primes, stored as seq_t */
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static uint64_t last_prime = 3;   /* largest prime that has been generated */

/* initialise primes list with 2 and 3 */
static void init_primes(void) {
    if (prime_cnt == 0) {
        grow_seq((void **)&primes, &prime_cap, 2);
        primes[0] = (seq_t)2;
        primes[1] = (seq_t)3;
        prime_cnt = 2;
        last_prime = 3;
    }
}

/* ensure primes up to at least n are present */
static void ensure_prime_list(uint64_t n) {
    for (uint64_t cand = last_prime + 1; cand <= n; ++cand) {
        if (is_prime_small((seq_t)cand)) {
            grow_seq((void **)&primes, &prime_cap, prime_cnt + 1);
            primes[prime_cnt++] = (seq_t)cand;
            last_prime = cand;
        }
    }
    if (last_prime < n) last_prime = n;   /* keep the loop invariant */
}

/* prime index (1‑based) of p in the primes list */
static uint64_t prime_index(uint64_t p) {
    for (size_t i = 0; i < prime_cnt; ++i) {
        if ((uint64_t)primes[i] == p) return (uint64_t)(i + 1);
    }
    return 0;  /* should never happen */
}

/* test whether an odd number x satisfies the required property */
static bool ok_number(uint64_t x) {
    if (is_prime_small((seq_t)x)) return true;

    /* factor x */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(x, pr, ex, &cnt);
    if (cnt == 0) return false;   /* shouldn't happen for x>1 */

    /* reject if any prime repeats (exponent > 1) */
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] > 1u) return false;

    /* collect indices of distinct prime factors */
    uint64_t idx[64];
    size_t ic = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t iidx = prime_index(pr[i]);
        if (iidx == 0) return false;   /* prime not in list */
        idx[ic++] = iidx;
    }

    /* pairwise coprime test */
    for (size_t i = 0; i < ic; ++i) {
        for (size_t j = i + 1; j < ic; ++j) {
            if (gcd64(idx[i], idx[j]) != 1u) return false;
        }
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for offset 1 */
    init_primes();

    static seq_t current_odd = (seq_t)3;

    while (cache_len < (size_t)n) {
        uint64_t cur = (uint64_t)current_odd;
        ensure_prime_list(cur);
        if (ok_number(cur)) {
            grow_seq((void **)&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = current_odd;
        }
        current_odd += (seq_t)2;
    }
    return cache[n - 1];
}
