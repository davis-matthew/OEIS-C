/*
 * A368633  Expansion of g.f. A(x) satisfying A(x) = 1 + 2*x*A(x)^2 -
 *          x*A(-x)^2.
 * https://oeis.org/A368633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, for n>0 if n is odd: a(n) = Σ_{i=0}^{n-1} a(i)*a(n-1-i)
 *            if n is even: a(n) = 3 * Σ_{i=0}^{n-1} a(i)*a(n-1-i)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the sequence iteratively and caches
 * previously computed values to support arbitrary call order.  The
 * computation is O(n²) time and uses only integer arithmetic,
 * which satisfies the requirements. */

#include <stdlib.h>

/* static cache for memoization */
static seq_t *cache = NULL;      /* array of a(i) */
static idx_t  cap   = 0;         /* allocated capacity */
static idx_t  sz    = 0;         /* number of valid entries (<= cap) */

static void ensure_capacity(idx_t needed)
{
    if (needed < cap) return;
    /* grow exponentially to avoid frequent reallocs */
    idx_t new_cap = cap ? cap * 2 : 4;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* realloc returns NULL only on failure; the problem assumes enough memory */
    cache = new_arr;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialize cache with a(0) = 1 */
    if (sz == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)1;
        sz = 1;
    }

    /* extend cache up to n */
    if (n >= sz) {
        ensure_capacity(n);
        for (idx_t k = sz; k <= n; ++k) {
            seq_t sum = (seq_t)0;
            /* compute convolution sum Σ a(i)*a(k-1-i) */
            for (idx_t i = 0; i <= k - 1; ++i) {
                sum += cache[i] * cache[k - 1 - i];
            }
            if ((k & 1) == 0) {      /* even k */
                cache[k] = sum * (seq_t)3;
            } else {                 /* odd k */
                cache[k] = sum;
            }
        }
        sz = n + 1;
    }

    return cache[n];
}
