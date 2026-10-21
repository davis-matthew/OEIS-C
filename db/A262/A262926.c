/*
 * A262926  Sum of n consecutive n-th powers starting with n^n.
 * https://oeis.org/A262926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   alloc_cache, big_pow, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = Σ_{k=n}^{2n-1} k^k      (OEIS A262926)  */
#include <stdlib.h>
#include "../utils/big_pow.h"      /* provides seq_t big_pow(seq_t base, seq_t exp) */
#include "../utils/free_cache.h"  /* provides void free_cache(seq_t *p)  */
#include "../utils/alloc_cache.h"/* provides seq_t *alloc_cache(idx_t n) */

static seq_t *pow_cache  = NULL;     /* pow_cache[i] = i^i  for i>=0  (0^0 = 0) */
static seq_t *pref_cache = NULL;     /* pref_cache[i] = Σ_{k=0}^{i} k^k   */
static idx_t  cache_size = 0;        /* number of elements currently allocated */

/*-----  Ensure that pow_cache and pref_cache contain values up to index m  -----*/
static void
ensure_pref_cache(idx_t m)
{
    if (m < cache_size)          /* already up‑to‑date */
        return;

    idx_t   old_size = cache_size;
    size_t  new_size = (size_t)(m + 1);   /* need indices 0 .. m */

    /* Allocate new arrays (using the provided allocator that handles
       arbitrary‑precision types correctly). */
    seq_t *new_pow  = alloc_cache(new_size);
    seq_t *new_pref = alloc_cache(new_size);
    if (!new_pow || !new_pref)           /* allocation failed – give up */
        return;

    /* Copy existing entries. */
    for (idx_t i = 0; i < old_size; ++i) {
        new_pow[i]  = pow_cache[i];
        new_pref[i] = pref_cache[i];
    }

    /* Compute new entries. */
    for (idx_t i = old_size; i < new_size; ++i) {
        if (i == 0) {                   /* 0^0 is defined as 0 in this sequence */
            new_pow[i]  = (seq_t)0;
            new_pref[i] = new_pow[i];
        } else {
            new_pow[i]  = big_pow((seq_t)i, (seq_t)i);
            new_pref[i] = new_pref[i - 1] + new_pow[i];
        }
    }

    /* Replace old caches by the new ones. */
    if (pow_cache)  free_cache(pow_cache);
    if (pref_cache) free_cache(pref_cache);
    pow_cache  = new_pow;
    pref_cache = new_pref;
    cache_size = new_size;
}

/*-------------------------------  Main function  ------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* an empty sum */

    idx_t m = 2 * n - 1;           /* n > 0 ensures no underflow */
    ensure_pref_cache(m);

    /* a(n) = pref_cache[2n-1] - pref_cache[n-1] */
    return pref_cache[m] - pref_cache[n - 1];
}
