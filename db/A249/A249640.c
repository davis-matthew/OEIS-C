/*
 * A249640  Number of strings of length n over a 7-letter alphabet that begin
 *          with a nontrivial palindrome.
 * https://oeis.org/A249640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   * a(0) = 0 * a(1) = 0 * a(n+1) = 7*a(n) + 7^ceil((n+1)/2) –
 *            a(ceil((n+1)/2))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static memoisation array – grows on demand */
    static seq_t *cache     = NULL;      /* computed values */
    static idx_t cache_size = 0;         /* allocated size */
    static idx_t computed   = 1;         /* highest index already computed */
    /* base values */
    if (computed <= 1) {
        if (cache_size == 0) {
            cache_size = 4;
            cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
            if (!cache) abort();
        }
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        computed = 1;
    }

    /* grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = cache_size ? cache_size : 4;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i) cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* trivial case */
    if (n <= 1) return cache[n];

    /* keep track of 7^k to avoid recomputing pow */
    static seq_t pow7 = (seq_t)1;   /* 7^0 */
    static idx_t pow_k = 0;         /* current exponent */

    for (idx_t i = computed + 1; i <= n; ++i) {
        /* ceil((i+1)/2) == (i+2)/2 with integer division */
        idx_t ceil_val = (i + 1) / 2;

        /* advance pow7 to 7^ceil_val */
        while (pow_k < ceil_val) {
            pow7 *= (seq_t)7;
            ++pow_k;
        }

        seq_t val = (seq_t)7 * cache[i - 1] + pow7 - cache[ceil_val];
        cache[i] = val;
    }
    computed = n;
    return cache[n];
}
