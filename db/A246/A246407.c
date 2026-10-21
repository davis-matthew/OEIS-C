/*
 * A246407  Nonnegative integers k satisfying sec(k) > sec(k+1).
 * https://oeis.org/A246407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A246407: nonnegative integers k such that sec(k) > sec(k+1) */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

/* Cache of computed sequence values */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* current capacity of the cache */
static uint64_t last_k_seen = 0;/* largest k that has been tested */

/* Return true if sec(k) > sec(k+1) holds for integer k */
static bool cond(uint64_t k)
{
    long double c1 = cosl((long double)k);          /* cos(k)    */
    long double c2 = cosl((long double)k + 1.0L);    /* cos(k+1)  */
    long double num = c2 - c1;                      /* numerator: c2 - c1  */
    long double den = c1 * c2;                      /* denominator: c1*c2 */

    /* sec(k) > sec(k+1)   ⇔   num / den  > 0
     * Since we only need the sign, check signs directly. */
    if (num > 0.0L)
        return den > 0.0L;
    if (num < 0.0L)
        return den < 0.0L;
    return false;   /* num can never be exactly 0 for integer k */
}

/* Ensure the cache has at least ‘needed’ places allocated. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate sequence values until at least ‘n’ terms are available. */
static void generate_until(idx_t n)
{
    if ((idx_t)n <= (idx_t)cache_size)
        return;

    ensure_capacity((size_t)n);

    /* Continue testing integers k until we have enough terms. */
    for (/* k starts where we left off */; cache_size < n; ++last_k_seen)
    {
        if (cond(last_k_seen))
        {
            cache[cache_size++] = (seq_t)last_k_seen;
        }
    }
}

/* Return the nth term (1‑based) of sequence A246407. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return 0;              /* undefined for n = 0; return 0 as sentinel */
    generate_until(n);
    return cache[n-1];
}
