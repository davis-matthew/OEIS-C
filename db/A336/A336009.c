/*
 * A336009  a(0) = ... = a(3) = 1; a(n) = Sum_{k=0..n-4} binomial(n-4,k) *
 *          a(k) * a(n-k-4).
 * https://oeis.org/A336009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=a(2)=a(3)=1; a(n) = Sum_{k=0}^{n-4} binomial(n-4,k) *
 *            a(k) * a(n-4-k)
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;   /* memoised values                     */
    static size_t cache_size = 0;     /* allocated length                    */
    static idx_t  computed_max = 0;   /* highest index that has been filled  */

    /* Allocate or grow the cache to hold indices 0 .. n */
    if (cache == NULL) {
        cache = seq_alloc_new_array((size_t)n + 1);
        cache_size = (size_t)n + 1;
    } else if (cache_size <= (size_t)n) {
        grow_seq_array(&cache, &cache_size, (size_t)n + 1);
    }

    /* Initialise a(0) on the very first allocation */
    if (computed_max == 0 && cache[0] == (seq_t)0) {
        cache[0] = (seq_t)1;
    }

    /* Compute any terms that have not yet been computed */
    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            if (i <= 3) {
                cache[i] = (seq_t)1;
            } else {
                seq_t sum = 0;
                idx_t limit = i - 4;
                for (idx_t k = 0; k <= limit; ++k) {
                    sum += binomial(limit, k) * cache[k] * cache[limit - k];
                }
                cache[i] = sum;
            }
        }
        computed_max = n;
    }

    return cache[n];
}
