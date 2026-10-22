/*
 * A392092  a(n+1) = a(n) + 2*d(a(n)) if s(a(n)) <= a(n), else 2*a(n) -
 *          s(a(n)), with a(1) = 2; d(k) is the number of divisors of k
 *          A000005 and s(k) is the aliquot sum of k A001065.
 * https://oeis.org/A392092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if s(a(n-1)) <= a(n-1) then a(n-1)+2*d(a(n-1)) else
 *            2*a(n-1)-s(a(n-1)) with a(1)=2; s(k)= sum of proper divisors of
 *            k; d(k)= number of divisors of k Note: the recurrence is
 *            evaluated for n>=2
 * Range:     u8 n <= 40, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Utilities for divisor count and sigma */
#include "../utils/divisor_count_u64.h"   /* size_t divisor_count_u64(uint64_t n); */
#include "../utils/sigma_of.h"           /* seq_t sigma_of(idx_t n); */

static seq_t *cache = NULL;          /* stores a[1]..a[cache_size] in cache[0..] */
static size_t cache_cap = 0;         /* allocated capacity in cache entries */
static size_t cache_size = 0;        /* number of terms currently stored */

/* Ensure cache has at least n entries allocated. */
static void ensure_cache(size_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    if (new_cap < n) new_cap = n;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) abort();          /* sequence defined only for n>=1 */
    /* compute up to n if needed */
    if (cache_size < n) {
        ensure_cache(n);
        /* compute missing terms */
        for (idx_t i = cache_size; i < n; ++i) {
            if (i == 0) {
                /* first term a(1) */
                cache[0] = (seq_t)2;
                continue;
            }
            seq_t prev = cache[i-1];
            /* Compute divisor count d(prev) */
            uint64_t prev64 = (uint64_t)prev;            /* assume fits */
            size_t d = divisor_count_u64(prev64);
            seq_t d_seq = (seq_t)d;
            /* Compute s(prev) = sigma(prev) - prev */
            seq_t sigma = sigma_of((idx_t)prev);
            seq_t s = sigma - prev;
            if (s <= prev) {
                cache[i] = prev + (seq_t)2 * d_seq;
            } else {
                cache[i] = (seq_t)2 * prev - s;
            }
        }
        cache_size = n;
    }
    return cache[n-1];
}
