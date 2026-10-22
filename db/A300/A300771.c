/*
 * A300771  Number of nX3 0..1 arrays with every element equal to 2, 3, 4 or 6
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) emp. recurrence of order 15 a(n) = 3*a(n‑1) + 3*a(n‑3) –
 *            3*a(n‑4) + 5*a(n‑5) – 2*a(n‑6) – 11*a(n‑7) – 13*a(n‑8) +
 *            11*a(n‑9) – 2*a(n‑10) – 5*a(n‑11) + a(n‑12) + 5*a(n‑13) –
 *            4*a(n‑14) + a(n‑15) Initial values: a(1)=0, a(2)=2, a(3)=3,
 *            a(4)=11, a(5)=32, a(6)=102, a(7)=330, a(8)=1059, a(9)=3418,...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;      /* capacity of cache array */
    static idx_t computed_len = 0;   /* largest n that has been computed (1‑based) */

    if (n < 1)
        return 0;                    /* offset is 1; n<1 is undefined */

    /* Ensure we have enough room for indices up to n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
        while (new_cap < n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                /* allocation failure – give up */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first 15 terms once */
    if (computed_len == 0) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)11;
        cache[5] = (seq_t)32;
        cache[6] = (seq_t)102;
        cache[7] = (seq_t)330;
        cache[8] = (seq_t)1059;
        cache[9] = (seq_t)3418;
        cache[10] = (seq_t)11023;
        cache[11] = (seq_t)35561;
        cache[12] = (seq_t)114744;
        cache[13] = (seq_t)370251;
        cache[14] = (seq_t)1194696;
        cache[15] = (seq_t)3854994;
        computed_len = 15;
    }

    /* Compute any missing terms up to n */
    for (idx_t k = computed_len + 1; k <= n; ++k) {
        seq_t t =  3 * cache[k - 1]
                 + 3 * cache[k - 3]
                 - 3 * cache[k - 4]
                 + 5 * cache[k - 5]
                 - 2 * cache[k - 6]
                 - 11 * cache[k - 7]
                 - 13 * cache[k - 8]
                 + 11 * cache[k - 9]
                 - 2 * cache[k -10]
                 - 5 * cache[k -11]
                 +  cache[k -12]
                 + 5 * cache[k -13]
                 - 4 * cache[k -14]
                 +  cache[k -15];
        cache[k] = t;
    }

    computed_len = (n > computed_len) ? n : computed_len;
    return cache[n];
}
