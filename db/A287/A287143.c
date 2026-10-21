/*
 * A287143  Expansion of x*(1 + 3*x + x^2)/((1 - x)^5*(1 + x)^4).
 * https://oeis.org/A287143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+4*a(n-2)-4*a(n-3)-6*a(n-4)+6*a(n-5) *
 *            +4*a(n-6)-4*a(n-7)-a(n-8)+a(n-9) * Initial values: a(0)=0 a(1)=1
 *            a(2)=4 a(3)=9 a(4)=21 * a(5)=35 a(6)=65 a(7)=95 a(8)=155
 *            a(9)=210
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;     /* number of elements currently stored */
    static idx_t cache_cap = 0;     /* allocated capacity */

    /* initialise cache with the first 10 terms */
    if (!cache) {
        cache_cap = 10;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)35;
        cache[6] = (seq_t)65;
        cache[7] = (seq_t)95;
        cache[8] = (seq_t)155;
        cache[9] = (seq_t)210;
        cache_len = 10;
    }

    /* extend cache up to n if necessary */
    if (n >= cache_len) {
        /* grow capacity if needed */
        idx_t need = n + 1;
        idx_t new_cap = cache_cap;
        while (new_cap < need) {
            new_cap <<= 1;
        }
        if (new_cap != cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing values */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val;
            if (i < 10) {
                val = cache[i];           /* base terms already present */
            } else {
                /* recurrence split into positive and negative parts to avoid
                   unsigned underflow (sum_pos >= sum_neg for all n). */
                seq_t sp = cache[i - 1] +
                           (seq_t)4 * cache[i - 2] +
                           (seq_t)6 * cache[i - 5] +
                           (seq_t)4 * cache[i - 6] +
                           cache[i - 9];
                seq_t sn = (seq_t)4 * cache[i - 3] +
                           (seq_t)6 * cache[i - 4] +
                           (seq_t)4 * cache[i - 7] +
                           cache[i - 8];
                val = sp - sn;
            }
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
