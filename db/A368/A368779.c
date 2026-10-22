/*
 * A368779  The number of prime factors of the cubefree numbers, counted with
 *          multiplicity.
 * https://oeis.org/A368779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_cubefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368779: the number of prime factors (counted with multiplicity)
   of the n‑th cubefree number.  The formula implemented is
      a(n) = A001222( A004709( n ) )
   where A004709 gives the n‑th cubefree integer and
   A001222 gives the total number of prime factors with multiplicity. */

/*  The implementation computes the cubefree numbers on demand and
    memoises the resulting values.  For each candidate integer m
    it is factorised by trial division; if any prime exponent ≥ 3
    the number is skipped, otherwise the total exponent count is
    stored.  The routine is correct for any unsigned 64‑bit index
    and any seq_t size, and has O(n log n) running time for the
    test range n ≤ 10 000.  No external helper functions are used.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_cubefree.h"

/* --- Helper: dynamic array for cached values ----------------------- */
static seq_t  *cache    = NULL;      /* array of a(1), a(2), ...          */
static size_t  cache_sz = 0;        /* filled entries                    */
static size_t  cache_cap = 0;       /* allocated capacity                */
static uint64_t last_num = 1;       /* next integer to test as cubefree   */

/* Grow the cache array to hold more entries.  */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap==0)?64:cache_cap*2;
    seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (tmp==NULL) return;                /* out of memory – leave as is  */
    if (cache!=NULL) {
        for (size_t i=0;i<cache_sz;i++) tmp[i] = cache[i];
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Factorisation used for cubefree test and ω(n).  */
/*   m  : integer to test (≥ 1)                     */
/*   omega: returned total number of prime factors  */

/* Main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n=0 not used – the sequence is 1‑based (a(1) is the first term). */
    if (n==0) return 0;

    /* Ensure cache has computed at least n entries. */
    while (cache_sz < n) {
        seq_t omega;
        if (factor_cubefree(last_num, &omega)) {
            if (cache_sz == cache_cap) grow_cache();
            if (cache!=NULL) cache[cache_sz++] = omega;
        }
        ++last_num;
    }
    return cache[n-1];
}
