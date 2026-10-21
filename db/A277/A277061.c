/*
 * A277061  Numbers with multiplicative digital root > 0.
 * https://oeis.org/A277061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   mult_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate successive integers and keep those whose multiplicative
 * digital root is never zero.  The root is computed by repeatedly
 * multiplying the decimal digits until a single digit remains; the
 * sequence consists of numbers for which this process never produces
 * a zero digit.  The function caches results so that arbitrary order
 * of calls is efficient. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mult_root.h"

/* Cached sequence and auxiliary data */
static seq_t *cache   = NULL;   /* array of found terms   */
static idx_t  cnt     = 0;      /* number of terms cached */
static idx_t  cap     = 0;      /* allocated capacity    */
static seq_t  next    = 1;      /* next integer to test  */

/* Append a new term to the cache, enlarging storage if necessary */
static void cache_append(seq_t val)
{
    if (cnt >= cap) {
        size_t new_cap = cap == 0 ? 16 : cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* fatal out‑of‑memory */
        cache = tmp;
        cap   = new_cap;
    }
    cache[cnt++] = val;
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but harmless */

    while (cnt < n) {
        if (mult_root(next) != 0)          /* keep only non‑zero roots */
            cache_append(next);
        ++next;                            /* advance to the next integer */
    }
    return cache[n - 1];
}
