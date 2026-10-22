/*
 * A343493  a(n) = 1 - Sum_{d|n, d < n} a(d - 1).
 * https://oeis.org/A343493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 - Sum_{d|n, d < n} a(d - 1)
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* a(n) with 0‑based indexing,  n >= 0  */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* current cache size (highest stored index + 1) */

    /* enlarge cache to hold values up to n */
    while (cap <= (size_t)n) {
        /* allocate or grow cache */
        if (cap == 0) {
            /* first allocation for a(0) */
            cache = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
            if (cache == NULL) return 0; /* fallback on allocation failure */
            /* we will fill entries progressively below */
        } else {
            if (cap < (size_t)n + 1) {
                seq_t *tmp = (seq_t*)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
                if (tmp == NULL) return 0;
                cache = tmp;
            }
        }

        /* compute a(cap) where cap currently is the next index to fill */
        if (cap == 0) {
            cache[0] = (seq_t)1;              /* a(0) = 1 */
            cap = 1;
            continue;
        }

        idx_t num = (idx_t)cap;              /* n = cap */
        seq_t sum = 0;

        /* enumerate proper divisors d of num */
        for (idx_t d = 1; d * d <= num; ++d) {
            if (num % d == 0) {
                idx_t d1 = d;
                if (d1 < num)               /* exclude d = n itself */
                    sum += cache[(size_t)(d1 - 1)];

                idx_t d2 = num / d;
                if (d2 != d1 && d2 < num)
                    sum += cache[(size_t)(d2 - 1)];
            }
        }

        cache[cap] = (seq_t)1 - sum;         /* a(n) = 1 - sum */
        cap++;
    }

    return cache[(size_t)n];
}
