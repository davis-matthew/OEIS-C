/*
 * A255634  Numbers n such that 1 + 16n^2 is prime.
 * https://oeis.org/A255634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 21, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255634: numbers m such that 1 + 16·m² is prime.
   The function returns the n‑th positive integer m for which 1 + 16·m² is prime. */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored qualifying numbers */
    static size_t cap = 0;        /* capacity of the cache       */
    static size_t sz  = 0;        /* number of elements stored   */
    static idx_t next = 1;        /* next integer to test        */

    while (sz < n) {
        if (sz == cap) {
            size_t new_cap = (cap == 0) ? 64 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap = new_cap;
        }

        seq_t cand   = (seq_t)next;
        seq_t test   = (seq_t)1 + (seq_t)16 * (cand * cand);

        if (is_prime_big(test))
            cache[sz++] = cand;

        ++next;
    }

    return cache[n - 1];
}
