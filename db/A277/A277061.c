/*
 * A277061  Numbers with multiplicative digital root > 0.
 * https://oeis.org/A277061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* Find the multiplicative digital root of x.
 * If a zero digit occurs during the process the function returns 0.
 * Otherwise it returns the final single digit (1‑9). */
static seq_t mult_root(seq_t x)
{
    uint64_t cur = (uint64_t)x;        /* the value to work on */

    while (cur >= 10)                 /* not yet a single decimal digit */
    {
        uint64_t prod = 1;
        while (cur > 0)
        {
            uint64_t d = cur % 10;
            if (d == 0)               /* a zero digit forces root 0 */
                return 0;
            prod *= d;
            cur /= 10;
        }
        cur = prod;                   /* next value in the chain */
    }
    return (seq_t)cur;
}

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
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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
