/*
 * A303723  Number of n X 4 0..1 arrays with every element unequal to 0, 1, 2
 *          or 3 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)-a(n-5)-2*a(n-6) for n>8,
 *            a(1)=8, a(2)=49, a(3)=46, a(4)=84, a(5)=133, a(6)=221, a(7)=406,
 *            a(8)=743
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 33, u64 n <= 70, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n<1, keep 0 */

    /* static cache, grows with the maximum n seen so far */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;     /* largest n for which cache[i] is valid */
    static idx_t cache_cap   = 0;     /* allocated size of cache (indices 0..cap-1) */

    /* Ensure we have space for indices up to n (inclusive) */
    if (n >= cache_cap)
    {
        idx_t new_cap = (n > 8 ? n + 1 : 9);   /* at least space for indices 1..8 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                    /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;

        /* If this is the first allocation, initialise the first 8 terms. */
        if (cache_len == 0)
        {
            cache[0] = 0;  /* unused, but fills the array cleanly */
            cache[1] = 8;  /* a(1) */
            cache[2] = 49; /* a(2) */
            cache[3] = 46; /* a(3) */
            cache[4] = 84; /* a(4) */
            cache[5] = 133;/* a(5) */
            cache[6] = 221;/* a(6) */
            cache[7] = 406;/* a(7) */
            cache[8] = 743;/* a(8) */
            cache_len = 8;
        }
    }

    /* Compute any missing entries up to n */
    for (idx_t k = cache_len + 1; k <= n; ++k)
    {
        if (k <= 8)
        {
            /* The first 8 terms are already set, but guard for future safety */
            switch (k) {
                case 1: cache[1] = 8;   break;
                case 2: cache[2] = 49;  break;
                case 3: cache[3] = 46;  break;
                case 4: cache[4] = 84;  break;
                case 5: cache[5] = 133; break;
                case 6: cache[6] = 221; break;
                case 7: cache[7] = 406; break;
                case 8: cache[8] = 743; break;
            }
        }
        else
        {
            /* Recurrence: a(k) = a(k-1)+a(k-2)+a(k-3)+a(k-4)-a(k-5)-2*a(k-6) */
            cache[k] = cache[k-1] + cache[k-2] + cache[k-3] + cache[k-4]
                      - cache[k-5] - (cache[k-6] << 1);
        }
    }

    cache_len = n;                    /* highest computed index */
    return cache[n];
}
