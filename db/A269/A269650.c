/*
 * A269650  Number of length-n 0..2 arrays with no adjacent pair x,x+1
 *          repeated.
 * https://oeis.org/A269650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 9*a(n-1) - 33*a(n-2) + 66*a(n-3) - 84*a(n-4)
                    + 75*a(n-5) - 47*a(n-6) + 21*a(n-7) - 6*a(n-8) + a(n-9)
   Initial values (offset 1): a(1)=3, a(2)=9, a(3)=27, a(4)=79,
   a(5)=225, a(6)=626, a(7)=1710, a(8)=4605, a(9)=12259 */

#include <stdlib.h>

static const int coeffs[9] = {9, -33, 66, -84, 75, -47, 21, -6, 1};
static const seq_t init_vals[9] = {
    (seq_t)3, (seq_t)9, (seq_t)27, (seq_t)79,
    (seq_t)225, (seq_t)626, (seq_t)1710,
    (seq_t)4605, (seq_t)12259
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset < 1 */

    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* initialise cache with first nine terms on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);   /* allocation failure */
        for (int i = 0; i < 9; ++i) cache[i] = init_vals[i];
        cache_size = 9;
    }

    /* if requested term is already cached, return it */
    if (n <= cache_size) return cache[n - 1];

    /* grow capacity if needed */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (n > new_cap) new_cap <<= 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(1);   /* allocation failure */
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* compute terms from current size up to n */
    for (idx_t i = cache_size; i < n; ++i) {
        seq_t term = 0;
        for (int j = 0; j < 9; ++j) {
            seq_t val = cache[i - j - 1];   /* a(i+1-j) */
            int c = coeffs[j];
            if (c >= 0)
                term += (seq_t)c * val;
            else
                term -= (seq_t)(-c) * val;
        }
        cache[i] = term;
    }
    cache_size = n;
    return cache[n - 1];
}
