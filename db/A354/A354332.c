/*
 * A354332  a(n) is the numerator of Sum_{k=0..n} (-1)^k / (2*k+1)!.
 * https://oeis.org/A354332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A354332: a(n) = numerator of Sum_{k=0..n} (-1)^k / (2*k+1)!   */
/* The function returns the reduced numerator when the fraction is
   written over the least common denominator (2*n+1)!   */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_gcd.h"

/* Cached reduced values of a(n).  */
static seq_t *cache = NULL;
static idx_t  cache_cap  = 0;   /* how many slots allocated          */
static idx_t  cache_last = 0;   /* highest index stored in cache      */
/* Values needed for the recurrence of the raw numerator.  */
static seq_t raw_A  = 0;   /* Raw numerator A_n = (2n+1)! * S_n   */
static seq_t raw_D  = 1;   /* Raw denominator (2n+1)!   */

/* Grow the cache array to hold indices up to n (inclusive).  */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the recursion for n = 0.  */
static void init(void)
{
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache_cap = 1;
        cache_last = 0;
        raw_A = (seq_t)1;
        raw_D = (seq_t)1;   /* (2*0+1)! = 1! = 1  */
    }
}

seq_t a(idx_t n)
{
    init();

    if (n <= cache_last) return cache[n];

    ensure_cache(n);

    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* Compute (2*i) and (2*i+1) as seq_t                                */
        seq_t two_i          = (seq_t)i * (seq_t)2;
        seq_t two_i_plus_one = two_i + (seq_t)1;

        /* Update raw denominator: D_i = D_{i-1} * 2*i * (2*i+1)             */
        raw_D = raw_D * two_i * two_i_plus_one;

        /* Update raw numerator: A_i = 2*i*(2*i+1)*A_{i-1} + (-1)^i         */
        seq_t prod = two_i * two_i_plus_one * raw_A;
        if (i & 1) prod = prod - (seq_t)1;   /* odd i → subtract 1      */
        else       prod = prod + (seq_t)1;   /* even i → add    1      */
        raw_A = prod;

        /* Reduce by gcd before storing                                    */
        seq_t g = seq_gcd(raw_A, raw_D);
        cache[i] = raw_A / g;

        cache_last = i;
    }
    return cache[n];
}
