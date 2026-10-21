/*
 * A258444  9-gonal numbers (A001106) that are the sum of twelve consecutive
 *          9-gonal numbers.
 * https://oeis.org/A258444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1, u128 n <= 3, big any n
 * Helpers:   grow_seq, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 1416317955*a(n-1) - 1416317955*a(n-2) + a(n-3)
   Offset 1.
   Initial values:
   a(1) = 1349094322576
   a(2) = 1910746510353532612000
   a(3) = 2706224588156555124000697809136
   The function memoises already computed terms to allow arbitrary
   access order.  It uses the supplied util functions for dynamic
   storage and string conversion. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Memoisation storage */
    static seq_t *cache = NULL;      /* array of computed terms */
    static size_t cap   = 0;         /* current capacity of cache */
    static idx_t  cached_cnt = 0;    /* number of terms stored */

    /* Base terms, parsed once */
    static bool   initialized = false;
    static seq_t a1, a2, a3;

    if (!initialized) {
        a1 = seq_from_str("1349094322576");
        a2 = seq_from_str("1910746510353532612000");
        a3 = seq_from_str("2706224588156555124000697809136");
        initialized = true;
    }

    /* Guard for indices below offset */
    if (n == 0) return 0;

    /* Ensure the cache holds the first three terms */
    if (cache == NULL) {
        /* Allocate space for at least three slots */
        grow_seq(&cache, &cap, 3);
        cache[0] = a1;
        cache[1] = a2;
        cache[2] = a3;
        cached_cnt = 3;
    }

    /* Extend cache up to requested index */
    while (cached_cnt < n) {
        /* Grow the array if necessary */
        if (cached_cnt >= cap)
            grow_seq(&cache, &cap, cached_cnt + 1);

        /* Compute next term using the recurrence */
        const seq_t K = (seq_t)1416317955;
        seq_t term = K * cache[cached_cnt - 1];
        term -= K * cache[cached_cnt - 2];
        term += cache[cached_cnt - 3];

        cache[cached_cnt] = term;
        ++cached_cnt;
    }

    return cache[n - 1];
}
