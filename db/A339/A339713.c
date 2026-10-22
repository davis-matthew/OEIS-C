/*
 * A339713  a(n) = (a(n-2) concatenate a(n-1)) for n > 2, with a(1)=1,
 *          a(2)=10.
 * https://oeis.org/A339713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(a(n-2), a(n-1)) for n>2, with a(1)=1, a(2)=10
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   digit_len, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>            /* malloc, realloc */
#include "../utils/pow10.h"    /* seq_t pow10(idx_t) */
#include "../utils/digit_len.h"/* int digit_len(seq_t) */
#include "../utils/seq_mul.h"  /* seq_t seq_mul(seq_t, seq_t) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* memoisation array, indices 0..computed_limit */
    static idx_t computed_limit = 0;  /* largest n that has been computed */

    /* n==0 is outside the OEIS offset; return 0 safely */
    if (n == 0)
        return (seq_t)0;

    /* initialise cache on first call: indices 1 and 2 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t)); /* indices 0,1,2 */
        if (cache == NULL)
            return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)0; /* unused placeholder */
        cache[1] = (seq_t)1; /* a(1) */
        cache[2] = (seq_t)10;/* a(2) */
        computed_limit = 2;
    }

    /* if requested term already cached, return it */
    if (n <= computed_limit)
        return cache[n];

    /* enlarge cache to hold up to index n */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (tmp == NULL)
        return (seq_t)0; /* allocation failed */
    cache = tmp;

    /* compute new terms from computed_limit+1 up to n */
    for (idx_t i = computed_limit + 1; i <= n; ++i) {
        /* length of decimal representation of a(i‑1) */
        int len = digit_len(cache[i - 1]);

        /* 10^len as seq_t */
        seq_t pow10_val = pow10((idx_t)len);

        /* a(i) = a(i-2) * 10^len(a(i-1)) + a(i-1) */
        seq_t mult = seq_mul(cache[i - 2], pow10_val);
        cache[i] = mult + cache[i - 1];
    }

    computed_limit = n;
    return cache[n];
}
