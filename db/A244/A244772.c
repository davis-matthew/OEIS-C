/*
 * A244772  Prime numbers ending in the prime number 59.
 * https://oeis.org/A244772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime of the form 100*k + 59
 * Range:     u8 n <= 1, u16 n <= 161, u32 n <= 162, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t cur_n = 0;
static idx_t next_k = 0;

seq_t a(idx_t n)
{
    /* Return the nth prime of the form 100*k + 59.
       n is 1-indexed.  The function builds the list lazily and
       memoises results, so calls may arrive in any order.    */
    while (cur_n < n) {
        seq_t cand = ((seq_t)100 * next_k) + 59;
        if (is_prime_big(cand)) {
            ensure_capacity(cur_n + 1, &cap, (void**)&cache, sizeof(seq_t));
            cache[cur_n] = cand;
            cur_n++;
        }
        next_k++;
    }
    return cache[n - 1];
}
