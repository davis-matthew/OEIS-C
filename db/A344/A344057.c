/*
 * A344057  a(n) = (2*n)! / CatalanNumber(n - 1) for n >= 1 and a(0) = 1.
 * https://oeis.org/A344057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)! / CatalanNumber(n - 1) Recurrence: a(0)=1, a(1)=2,
 *            a(n)=a(n‑1)*n²*(2*n‑1)/(2*n‑3) for n>1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t _egcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache_len == 0) {                         /* initialise cache[0] */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) /* malloc failed, abort */ return (seq_t)0;
        cache_len = 1;
        cache[0] = (seq_t)1;
    }

    if (n >= cache_len) {                         /* extend cache up to n */
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                /* realloc failed */
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 1) {                         /* a(1)=2 */
                cache[1] = (seq_t)2;
                continue;
            }

            /* recurrence */
            seq_t prev = cache[i-1];
            idx_t ii   = i;

            /* factors */
            seq_t den   = (seq_t)(2*ii - 3);
            seq_t num1  = (seq_t)ii * (seq_t)ii;      /* i² */
            seq_t num2  = (seq_t)(2*ii - 1);           /* 2i‑1 */

            /* reduce intermediate overflows by cancelling gcds */
            seq_t g = _egcd(prev, den);
            prev /= g;
            den  /= g;

            g = _egcd(num1, den);
            num1 /= g;
            den  /= g;

            g = _egcd(num2, den);
            num2 /= g;
            den  /= g;
            /* after divisions den should be 1 (exact division) */

            seq_t val = prev;
            val *= num1;
            val *= num2;

            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}
