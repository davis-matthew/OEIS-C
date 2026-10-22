/*
 * A340419  Trajectory of 397 under the map A340008: n -> n/2 if n is even,
 *          n-> n^2 - 1 if n is an odd prime, otherwise n -> n - 1.
 * https://oeis.org/A340419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 0, u32 n <= 75, u64 n <= 113, u128 n <= 125,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Trajectory of 397 under the map
   a(0) = 397
   a(n+1) = a(n)/2   if a(n) is even
            a(n)^2-1 if a(n) is odd prime
            a(n)-1   if a(n) is odd composite
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;   /* cached sequence values */
static size_t cap = 0;        /* current allocated capacity */
static idx_t sz = 0;          /* number of terms stored */

/* Ensure the cache is initialized with the initial term a(0)=397 */
static void init_cache(void)
{
    if (!cache) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);                     /* allocation failed */
        cache[0] = (seq_t)397;
        sz = 1;
    }
}

/* Return the nth term of the sequence, computing and memoizing as needed */
seq_t a(idx_t n)
{
    init_cache();

    /* If the requested term is not yet computed, extend the cache and
       fill in the missing values. */
    if (n >= sz) {
        size_t needed = (size_t)n + 1;
        if (needed > cap) {
            size_t newcap = cap;
            while (newcap < needed) newcap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) exit(1);                 /* allocation failed */
            cache = tmp;
            cap   = newcap;
        }

        /* Compute terms from the current size up to n */
        for (idx_t i = sz; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t next;
            if ((prev % 2) == 0) {            /* even */
                next = prev / 2;
            } else {                         /* odd */
                if (is_prime_big(prev)) {    /* odd prime */
                    next = prev * prev - 1;
                } else {                     /* odd composite */
                    next = prev - 1;
                }
            }
            cache[i] = next;
        }
        sz = n + 1;
    }

    return cache[n];
}
