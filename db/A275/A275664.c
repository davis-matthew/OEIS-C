/*
 * A275664  a(n) is the sum of the LCM and GCD of all previous terms, with
 *          a(0) = 2.
 * https://oeis.org/A275664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=2; a(n+1)=lcm(a(0..n))+gcd(a(0..n))
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   gcd, grow_cache_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/grow_cache_to.h"

/* Helper to compute the lcm of two seq_t values */
static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    if (a == 0 || b == 0) return 0;
    seq_t g = gcd(a, b);
    seq_t div = a / g;
    return div * b;
}

/* The sequence function */
seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static seq_t last_lcm = 0;
    static seq_t last_gcd = 0;

    /* initialise the sequence on first use */
    if (cache_size == 0) {
        size_t ini_cap = 1;
        grow_cache_to(ini_cap, &cache, &cache_cap, 0);
        last_lcm = (seq_t)2;
        last_gcd = (seq_t)2;
        cache[0] = (seq_t)2;
        cache_size = 1;
    }

    /* already computed? */
    if ((idx_t)cache_size > n) return cache[n];

    /* extend until we have a(n) */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        /* compute next term from current lcm and gcd */
        seq_t next = last_lcm + last_gcd;

        /* grow cache if needed */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap <= cache_size) new_cap *= 2;
            grow_cache_to(new_cap, &cache, &cache_cap, cache_size);
        }

        cache[cache_size] = next;

        /* update lcm and gcd for the next step */
        seq_t new_lcm = lcm_seq(last_lcm, next);
        seq_t new_gcd = gcd(last_gcd, next);
        last_lcm = new_lcm;
        last_gcd = new_gcd;

        ++cache_size;
    }

    return cache[n];
}
