/*
 * A305035  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2, 6
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) for n>6 initial values: a(1)=4, a(2)=3,
 *            a(3)=7, a(4)=7, a(5)=13, a(6)=23
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 42, u64 n <= 84, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>       /* malloc, realloc, NULL */

/* Cache of previously computed terms; cache[i] = a(i+1)   (0‑indexed) */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;   /* number of cached values */

/* Small table for the first six terms. */
static const seq_t base_vals[6] = {
    (seq_t)4, (seq_t)3, (seq_t)7,
    (seq_t)7, (seq_t)13, (seq_t)23
};

seq_t a(idx_t n)
{
    /* Undefined for n<1; returning 0 is safe and harmless. */
    if (n == 0) return (seq_t)0;

    /* If the requested value is already cached, return it. */
    if (n <= cache_len) return cache[n-1];

    /* Grow the cache up to n entries. */
    idx_t new_len = n;
    /* realloc needs a void* return in C++; cast it to seq_t* */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
    if (!tmp) {        /* allocation failure – leave cache unchanged */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute new entries, starting from cache_len+1 up to n. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t val;
        if (k <= 6) {
            val = base_vals[k-1];
        } else {
            /* a(k) = a(k-1) + 2*a(k-3) */
            val = cache[(k-1)-1] + (seq_t)2 * cache[(k-3)-1];
        }
        cache[k-1] = val;
    }

    cache_len = new_len;
    return cache[n-1];
}
