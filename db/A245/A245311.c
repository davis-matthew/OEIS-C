/*
 * A245311  G.f. satisfies: A(x) = 1 + x*A(x) + x^2*A(x)^2 + x^3*A(x)*A'(x).
 * https://oeis.org/A245311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 29, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A245311
   Recurrence derived from the functional equation:
        A(x) = 1 + x*A + x^2*A^2 + x^3*A*A'
   Coefficient form (for n >= 1):
        a_n = a_{n-1}
              + sum_{k=0}^{n-2} a_k * a_{n-2-k}
              + sum_{k=0}^{n-2} a_k * a_{n-2-k} * (n-2-k)
   Initial values: a_0 = 1 , a_1 = 1
   Arguments: n is the index, the sequence is memoised in static storage.
   Works for all integer types seq_t supplied by the build. */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* memoised values a(0)...a(n)   */
static idx_t cache_cap = 0;          /* capacity allocated for cache  */
static idx_t max_computed = 0;       /* largest n that has been computed */

seq_t a(idx_t n)
{
    /* Allocate/expand cache to hold up to index n */
    if (n + 1 > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? (size_t)(cache_cap * 2) : 16;
        while ((size_t)(new_cap) <= n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory – return 0  */
        /* Initialise newly allocated entries to zero (works for all seq_t) */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise a(0) and a(1) if not already done */
    if (max_computed < 1) {
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache[1] = (seq_t)1;          /* a(1) = 1 */
        max_computed = 1;
    } else if (max_computed < 0) {
        cache[0] = (seq_t)1;
        max_computed = 0;
    }

    /* If the requested term is already computed, return it */
    if (n <= max_computed) return cache[n];

    /* Compute terms from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t C = 0;   /* sum_{k=0}^{i-2} a_k * a_{i-2-k}          */
        seq_t D = 0;   /* sum_{k=0}^{i-2} a_k * a_{i-2-k} * (i-2-k) */
        idx_t lim = i >= 2 ? i - 2 : 0;
        for (idx_t k = 0; k <= lim; ++k) {
            seq_t prod = cache[k] * cache[lim - k];
            C += prod;
            D += prod * (seq_t)(lim - k);
        }
        cache[i] = cache[i - 1] + C + D;
    }
    max_computed = n;
    return cache[n];
}
