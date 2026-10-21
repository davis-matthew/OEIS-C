/*
 * A277495  a(1) = 1; a(n) = n if a(n-1) = 1; otherwise, a(n) = digit sum (n
 *          written in base a(n-1)).
 * https://oeis.org/A277495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; a(n) = n if a(n-1) = 1; a(n) = digit_sum(n in base
 *            a(n-1)) otherwise (digit_sum = sum of base‑a(n-1) digits of n)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holds a(1)...a(stored) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated slots (≥ stored) */
    static idx_t  stored = 0;  /* number of terms stored (≥1) */

    if (n == 0) return 0;   /* undefined, but safe fallback */

    /* lazy initialization */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure */
        cache[0] = (seq_t)1;            /* a(1) = 1 */
        stored   = 1;
    }

    /* extend cache if needed */
    if (stored < n) {
        if (n > cap) {
            size_t newcap = cap;
            while (newcap < n) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) {
                free(cache);
                cache = NULL;
                return 0;
            }
            cache = tmp;
            cap   = newcap;
        }

        /* compute missing terms in order */
        for (idx_t i = stored + 1; i <= n; ++i) {
            seq_t prev = cache[i - 2];   /* a(i-1) */
            seq_t val;

            if (prev == (seq_t)1) {
                val = (seq_t)i;
            } else {
                seq_t num  = (seq_t)i;
                seq_t sum  = 0;
                seq_t base = prev;
                /* digit sum of num in base 'base' */
                while (num > 0) {
                    sum += num % base;
                    num  /= base;
                }
                val = sum;
            }
            cache[i - 1] = val;
        }
        stored = n;
    }

    return cache[n - 1];
}
