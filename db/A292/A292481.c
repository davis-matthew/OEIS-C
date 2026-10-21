/*
 * A292481  p-INVERT of the odd positive integers, where p(S) = 1 - S^3.
 * https://oeis.org/A292481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=1, a(3)=9, a(4)=42, a(5)=139; a(n) =
 *            6*a(n-1) - 15*a(n-2) + 21*a(n-3) - 12*a(n-4) + 9*a(n-5) for n>=6
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;   /* array of seq_t */
    static size_t cache_size = 0; /* allocated size of cache   */
    static size_t last_computed = 5; /* largest index computed so far */

    /* initialise cache on first call */
    if (!cache) {
        cache_size = 8;               /* small starting size */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) {                 /* allocation failed */
            exit(EXIT_FAILURE);
        }
        /* predefined first 6 terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)42;
        cache[5] = (seq_t)139;
        /* remaining allocated entries initialised to 0 */
        for (size_t i = 6; i < cache_size; ++i) cache[i] = 0;
    }

    /* grow cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_size = cache_size;
        while (new_size <= (size_t)n) new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {                 /* allocation failed */
            exit(EXIT_FAILURE);
        }
        /* initialise new entries to zero */
        for (size_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    if ((size_t)n > last_computed) {
        size_t start = last_computed + 1;
        if (start < 6) start = 6;   /* the first 6 terms are already known */
        for (size_t i = start; i <= (size_t)n; ++i) {
            seq_t pos = ((seq_t)6) * cache[i-1]
                       + ((seq_t)21) * cache[i-3]
                       + ((seq_t)9)  * cache[i-5];
            seq_t neg = ((seq_t)15) * cache[i-2]
                       + ((seq_t)12) * cache[i-4];
            cache[i] = pos - neg;   /* pos≥neg for all n≥6, so unsigned subtraction is safe */
        }
        last_computed = (size_t)n;
    }

    return cache[n];
}
