/*
 * A302091  Primes of form 6*k^k + 5.
 * https://oeis.org/A302091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*k^k + 5 where k is the smallest integer such that 6*k^k
 *            + 5 is a prime; a(1) is the first such prime. This implements
 *            OEIS sequence A302091 with offset 1
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, grow_seq_array, is_prime_big, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#include "../utils/big_pow.h"
#include "../utils/mul_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq_array.h"

static seq_t   *cache     = NULL;   /* dynamic array of primes found so far */
static size_t  cache_len  = 0;      /* number of primes stored in cache   */
static size_t  cache_cap  = 0;      /* allocated capacity of cache       */
static uint64_t k_next     = 1;      /* next k to test */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for offset 0; return 0 to avoid UB. */
        return (seq_t)0;
    }

    /* Extend cache until it contains at least n primes. */
    while (cache_len < (size_t)n) {
        /* Find next prime of the form 6*k^k + 5. */
        for (;;) {
            seq_t base = (seq_t)k_next;
            seq_t exp  = (seq_t)k_next;

            seq_t val = big_pow(base, exp);      /* k^k          */
            val = mul_small(val, 6);              /* 6*k^k        */
            val = val + (seq_t)5;                 /* 6*k^k + 5    */

            if (is_prime_big(val)) {
                /* Grow cache if needed and store the new prime. */
                grow_seq_array(&cache, &cache_cap, cache_len + 1);
                cache[cache_len] = val;
                cache_len++;

                /* Advance k_next for next search. */
                k_next++;
                break; /* found a prime, move on to the next one. */
            }
            /* Not prime, test the next k. */
            k_next++;
        }
    }

    return cache[n - 1];
}
