/*
 * A275498  Number of n X 2 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,0) or (-1,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=9, a(3)=16, a(4)=28, a(5)=56, a(6)=104,
 *            a(n)=a(n-1)+2*a(n-2)-a(n-4) for n>6
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 33, u64 n <= 67, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* static cache for computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t max_computed = 0;

/* initial 6 values taken from the OEIS entry */
static const seq_t base[6] = {
    (seq_t)2,
    (seq_t)9,
    (seq_t)16,
    (seq_t)28,
    (seq_t)56,
    (seq_t)104
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n<1, but defensively return 0 */

    /* ensure cache is large enough for index n */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);   /* need indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();    /* allocation failure */
        cache = tmp;
        cache_size = (idx_t)new_size;
    }

    /* already computed? */
    if (n <= max_computed) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = base[i - 1];
        } else {
            /* a(i) = a(i-1) + 2*a(i-2) - a(i-4) */
            seq_t twice = cache[i - 2] + cache[i - 2];
            cache[i] = cache[i - 1] + twice - cache[i - 4];
        }
    }
    max_computed = n;
    return cache[n];
}
