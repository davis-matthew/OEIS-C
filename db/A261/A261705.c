/*
 * A261705  Number of (n+2)X(2+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 00000101 or 00000111.
 * https://oeis.org/A261705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) – a(n-3) – 6*a(n-5) + 4*a(n-6) * –
 *            5*a(n-7) + 2*a(n-8) + 11*a(n-9) – 3*a(n-10) * – 2*a(n-12) –
 *            6*a(n-14) + 4*a(n-15) for n > 17 * Initial values (n = 1 … 17):
 *            * 76, 134, 270, 462, 892, 1751, 3299, 6324, 12389, * 23874,
 *            46352, 90232, 175147, 340107, 662278, * 1287201, 2502792
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 28, u64 n <= 61, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache memoization for arbitrary order calls */
static seq_t *cache = NULL;          /* indices 1 .. cache_cap are valid  */
static idx_t cache_cap = 0;          /* current maximum index stored      */

/* 1‑based indexing: a(0) is undefined, return 0 for safety   */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* enlarge cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = n;
        seq_t *tmp = (cache_cap == 0)
            ? (seq_t *)malloc((new_cap + 1) * sizeof(seq_t))
            : (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));

        if (!tmp) /* allocation failed */
            return 0;
        cache = tmp;

        /* initial terms */
        static const seq_t init[17] = {
            (seq_t)76,     (seq_t)134,    (seq_t)270,    (seq_t)462,
            (seq_t)892,    (seq_t)1751,   (seq_t)3299,   (seq_t)6324,
            (seq_t)12389,  (seq_t)23874,  (seq_t)46352,  (seq_t)90232,
            (seq_t)175147, (seq_t)340107, (seq_t)662278, (seq_t)1287201,
            (seq_t)2502792
        };

        /* fill new entries */
        for (idx_t i = cache_cap + 1; i <= new_cap; ++i) {
            if (i <= 17) {
                cache[i] = init[i - 1];
            } else {
                /* compute recurrence avoiding intermediate underflow */
                seq_t pos = cache[i - 1] + cache[i - 1]               /* +2*a(n-1) */
                          + cache[i - 2]                              /* +a(n-2) */
                          + cache[i - 6] * (seq_t)4                  /* +4*a(n-6) */
                          + cache[i - 8] * (seq_t)2                  /* +2*a(n-8) */
                          + cache[i - 9] * (seq_t)11                 /* +11*a(n-9) */
                          + cache[i - 15] * (seq_t)4;                /* +4*a(n-15) */

                seq_t neg = cache[i - 3]                           /* -a(n-3) */
                          + cache[i - 5] * (seq_t)6                 /* -6*a(n-5) */
                          + cache[i - 7] * (seq_t)5                 /* -5*a(n-7) */
                          + cache[i - 10] * (seq_t)3                /* -3*a(n-10) */
                          + cache[i - 12] * (seq_t)2                /* -2*a(n-12) */
                          + cache[i - 14] * (seq_t)6;               /* -6*a(n-14) */

                cache[i] = pos - neg;   /* pos > neg for all n */
            }
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
