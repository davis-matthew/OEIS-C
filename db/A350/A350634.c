/*
 * A350634  Products of the parts s,t in each partition of k (= 2,3,...) into
 *          two parts, ordered by increasing k and then by increasing values
 *          of s*t (see example).
 * https://oeis.org/A350634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the product s*t of all unordered partitions of k
 *            (=2,3,…) into * two parts, ordered first by increasing k and
 *            then by increasing s*t. * For each k, the products are s*(k-s)
 *            for s = 1..⌊k/2⌋, which are naturally * in increasing order
 *            because s*(k-s) increases with s up to k/2
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed terms */
    static size_t   cache_cap = 0;   /* current capacity */
    static size_t   cache_len = 0;   /* number of terms stored */

    /* state of the generator */
    static idx_t cur_k = 2;          /* current k */
    static idx_t cur_s = 1;          /* current s */

    /* Ensure the cache can hold n terms */
    if (n == 0) return 0; /* undefined for n < 1, but guard */
    if (cache_len < n) {
        /* grow the cache if needed */
        size_t needed = n;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();  /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* generate additional terms until reaching n */
        while (cache_len < n) {
            seq_t prod = (seq_t)cur_s * (seq_t)(cur_k - cur_s);
            cache[cache_len++] = prod;
            /* advance to next pair */
            cur_s++;
            if (cur_s > cur_k / 2) {   /* finished this k */
                cur_s   = 1;
                cur_k++;
            }
        }
    }
    return cache[n - 1];
}
