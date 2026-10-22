/*
 * A367875  Expansion of e.g.f. exp(x * (3 + exp(x))).
 * https://oeis.org/A367875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   binomial_inline
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1;
   a(n)=3*a(n-1)+∑_{k=1..n} binomial(n-1,k-1)*k*a(n-k). */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial_inline.h"

static seq_t *cache = NULL;      /* Memoised a(n) values          */
static size_t cache_cap = 0;     /* Allocated array capacity      */
static idx_t computed = 0;       /* Number of values computed     */

seq_t a(idx_t n)
{
    /* Allocate or grow cache to hold up to index n */
    if ((size_t)(n + 1) > cache_cap) {
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        cache_cap = (size_t)(n + 1);
    }

    for (idx_t i = computed; i <= n; ++i) {
        if (i == (idx_t)0) {
            cache[0] = (seq_t)1;           /* a(0) = 1 */
        } else {
            seq_t sum = cache[i - 1] * (seq_t)3;
            for (idx_t k = (idx_t)1; k <= i; ++k) {
                seq_t bin = binomial_inline(i - 1, k - 1);
                seq_t term = bin;
                term *= (seq_t)k;
                term *= cache[i - k];
                sum += term;
            }
            cache[i] = sum;
        }
    }
    computed = n + 1;   /* All values up to n have been computed */
    return cache[n];
}
