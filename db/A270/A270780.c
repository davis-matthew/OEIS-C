/*
 * A270780  Let p_i = the i-th prime. a(i) is the smallest n>1 such that p_i
 *          divides n!-1.
 * https://oeis.org/A270780
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   ensure_cache, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A270780: a(n) = smallest m>1 such that the n-th prime p divides m! – 1,
   offset 3.  A search for the minimal m is performed via factorial
   computation modulo p. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/ensure_cache.h"

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    /* sequence is undefined for n<3; return 0 as a sentinel */
    if (n < 3) return (seq_t)0;

    /* static cache of previously computed values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;

    /* make sure we have space for the requested index */
    ensure_cache(n, &cache, &cache_cap);

    /* use 0 as “uncomputed” sentinel (minimal value is 2) */
    if (cache[n] != (seq_t)0) return cache[n];

    /* fetch the nth prime (primes are 1‑based) */
    uint32_t p = get_prime(n);       /* p ≥ 5 for n ≥ 3 */

    /* compute m! modulo p until it becomes 1 (m>1) */
    seq_t cur = (seq_t)(2 % p);      /* 2! mod p */

    if (cur == (seq_t)1) {
        cache[n] = (seq_t)2;
        return cache[n];
    }

    /* iterate m = 3 … p‑2 (by Wilson’s theorem a solution always exists) */
    for (idx_t m = 3; m <= (idx_t)(p - 2); ++m) {
        uint64_t prod = (uint64_t)cur * (uint64_t)m;   /* avoid overflow */
        cur = (seq_t)(prod % (uint64_t)p);

        if (cur == (seq_t)1) {
            cache[n] = (seq_t)m;
            return cache[n];
        }
    }

    /* fallback: the guaranteed solution is p‑2 */
    cache[n] = (seq_t)(p - 2);
    return cache[n];
}
