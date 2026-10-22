/*
 * A392559  Numbers x such that for some y, a divisor of x such that 1<y<x,
 *          the following four numbers are prime: x*y+1, x*y-1, x/y+1, x/y-1.
 * https://oeis.org/A392559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 36, u32 n <= 41, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A392559
 *
 * This implementation searches successive multiples of 6 (starting at 30)
 * for the first n numbers that satisfy the definition:
 *
 *   For some divisor 1 < y < x, all of
 *       x*y + 1,  x*y - 1,
 *       x/y + 1,  x/y - 1
 *   are prime.
 *
 * With the help of the routine is_prime_big from the utils,
 * the search is carried out incrementally and the results are cached
 * so that calls in arbitrary order are efficient.
 *
 * The sequence terms are all multiples of 6, starting with 30.
 *
 * The function has static storage and is fully re‑entrant for any
 * order of calls.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* cache of found terms          */
static size_t cache_len = 0;     /* number of terms stored        */
static size_t cache_cap = 0;     /* allocated capacity            */
static seq_t next_candidate = 30;/* next multiple of 6 to test    */

/* Ensure cache has space for at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Test whether a candidate x satisfies the definition
 * for some divisor y.  All calculations stay in seq_t. */
static bool satisfies(seq_t x)
{
    /* iterate over divisors y > 1, y*y <= x */
    seq_t y;
    for (y = 2; y * y <= x; ++y)
    {
        if (x % y != 0) continue;

        /* divisors: y and k = x / y */
        seq_t k = x / y;
        if (k <= 1) continue;    /* k must be >1 */

        /* First test with divisor y */
        if (is_prime_big(k - 1) &&
            is_prime_big(k + 1) &&
            is_prime_big(y * y * k - 1) &&
            is_prime_big(y * y * k + 1))
        {
            return true;
        }

        /* If k differs from y, test with divisor k */
        if (k != y && y <= x / k) /* ensure k not already tested when y>sqrt(x) */
        {
            seq_t y2 = k;   /* new divisor */
            seq_t k2 = x / y2; /* which equals y */

            if (is_prime_big(k2 - 1) &&
                is_prime_big(k2 + 1) &&
                is_prime_big(y2 * y2 * k2 - 1) &&
                is_prime_big(y2 * y2 * k2 + 1))
            {
                return true;
            }
        }
    }
    return false;
}

/* Main sequence routine.  Any order of calls is supported. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, harmless */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        /* Find the next valid multiple of 6 */
        seq_t found = 0;
        for (;;)
        {
            if (satisfies(next_candidate))
            {
                found = next_candidate;
                break;
            }
            next_candidate += 6;
        }

        /* Store the found term */
        grow_cache(cache_len + 1);
        cache[cache_len++] = found;

        /* Prepare for next search */
        next_candidate = found + 6;
    }

    return cache[(size_t)n - 1];
}
