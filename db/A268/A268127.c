/*
 * A268127  a(n) = (A005704(n)-A006047(n))/3.
 * https://oeis.org/A268127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268127: a(n) = (A005704(n)-A006047(n))/3
   A005704: b(0)=1, b(n)=b(n-1)+b(floor(n/3))
   A006047: c(n)=∏_{digits d of n in base 3}(d+1)   (c(0)=1)
   a(n)= (b(n)-c(n))/3   (valid for n≥0)
   Uses a static cache for b and c; grows lazily to any requested n.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for b(n) and c(n) */
    static seq_t *b_cache = NULL;
    static seq_t *c_cache = NULL;
    static idx_t cache_size   = 0; /* allocated array size */
    static idx_t max_n_computed = 0; /* largest n for which values are filled */

    /* ensure the cache can hold index n */
    idx_t needed = n + 1;
    if (cache_size < needed) {
        idx_t new_size = cache_size ? cache_size << 1 : 1;
        while (new_size < needed) new_size <<= 1;
        b_cache = (seq_t *)realloc(b_cache, new_size * sizeof(seq_t));
        c_cache = (seq_t *)realloc(c_cache, new_size * sizeof(seq_t));
        cache_size = new_size;
        /* initialise base case if it hasn't been set yet */
        if (max_n_computed == 0) {
            b_cache[0] = 1;
            c_cache[0] = 1;
        }
    }

    /* compute missing values up to n */
    for (idx_t i = max_n_computed + 1; i <= n; ++i) {
        /* b(i) = b(i-1)+b(floor(i/3)) */
        b_cache[i] = b_cache[i - 1] + b_cache[i / 3];

        /* c(i) = ∏(digit+1) for ternary digits of i */
        seq_t prod = 1;
        idx_t t = i;
        while (t > 0) {
            prod *= (seq_t)(t % 3 + 1);
            t /= 3;
        }
        c_cache[i] = prod;
    }
    if (n > max_n_computed) {
        max_n_computed = n;
    }

    /* a(n) = (b(n)-c(n))/3 */
    return (b_cache[n] - c_cache[n]) / 3;
}
