/*
 * A248203  Numbers n such that n-1, n, and n+1 are the product of 4 distinct
 *          primes.
 * https://oeis.org/A248203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 300, u64 n <= 300, u128 n <= 300,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A248203:
   a(n) = the smallest integer m such that m-1, m, and m+1 are all
   square‑free products of exactly four distinct primes.
   The function searches incrementally and memoises found terms.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Return true if n is the product of exactly four distinct, square‑free
   primes. */
static bool is_quadruple_product(uint64_t n)
{
    /* factor_u64 stores the prime factors of n in primes[] with exponents
       in exps[], and returns the count in cnt. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    if (cnt != 4) return false;          /* need 4 distinct primes */
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] != 1) return false;  /* must be square‑free */
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static uint64_t next_test = 3;        /* what to test next */

    while (cache_len < n) {
        uint64_t cand = next_test;
        ++next_test;

        if (is_quadruple_product(cand - 1) &&
            is_quadruple_product(cand)     &&
            is_quadruple_product(cand + 1)) {

            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_buf) return (seq_t)0;  /* allocation failure */
                cache   = new_buf;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }
    return cache[n - 1];
}
