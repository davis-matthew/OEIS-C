/*
 * A327439  a(0)=1. If a(n-1) and n are relatively prime and a(n-1)!=1, a(n) =
 *          a(n-1) - 1. Otherwise (i.e., if a(n-1) and n share a common factor
 *          or a(n-1)=1), a(n) = a(n-1) + gcd(a(n-1),n) + 1.
 * https://oeis.org/A327439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 665, u16 n <= 9888, u32 n <= 9888, u64 n <= 9888,
 *            u128 n <= 9888, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>0
   let g = gcd(a(n‑1), n);
   if (a(n‑1) != 1 && g == 1) a(n) = a(n‑1) – 1;
   else                 a(n) = a(n‑1) + g + 1
   This defines OEIS sequence A327439.
*/

#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Forward declaration of the function defined below. */
seq_t a(idx_t n);

/* Static cache of computed terms. */
static seq_t *cache   = NULL;   /* array of terms */
static size_t  cap    = 0;      /* current allocation size */
static size_t  sz     = 0;      /* number of terms already stored */

/* Ensure that all terms up to ‘upto’ (inclusive) are computed and stored. */
static void ensure_cache(idx_t upto)
{
    /* Allocate initial storage if needed. */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failed */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        sz = 1;
    }

    /* Nothing to do if the requested index is already computed. */
    if ((idx_t)(sz - 1) >= upto)
        return;

    /* Grow the array if necessary. */
    if ((idx_t)cap <= upto) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)upto)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms. */
    for (size_t idx = sz; idx <= (size_t)upto; ++idx) {
        seq_t prev = cache[idx - 1];
        seq_t g = gcd(prev, (seq_t)idx); /* gcd(prev, n) */
        if (prev != (seq_t)1 && g == (seq_t)1)
            cache[idx] = prev - (seq_t)1;
        else
            cache[idx] = prev + g + (seq_t)1;
    }
    sz = (size_t)upto + 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
