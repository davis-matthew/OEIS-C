/*
 * A254820  Number of length n 1..(1+1) arrays with every leading partial sum
 *          divisible by 2, 3 or 5.
 * https://oeis.org/A254820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 37, u32 n <= 75, u64 n <= 152,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A254820 – number of length‑n arrays with entries 1..2 such that all
    leading partial sums are divisible by 2, 3 or 5.

    Recurrence for n ≥ 23:
        a(n) = a(n‑15) + 9*a(n‑16) + 32*a(n‑17) + 58*a(n‑18)
             + 58*a(n‑19) + 32*a(n‑20) + 9*a(n‑21) + a(n‑22)

    Initial values (n = 1 … 22):
        1, 2, 4, 5, 6, 9, 10, 11, 15, 19, 21, 26, 35, 48, 75,
        120, 166, 203, 262, 395, 613, 879
*/

#include <stdlib.h>   /* malloc, realloc                */

seq_t a(idx_t n)
{
    /* ---------  constants  --------- */
    /* initial 22 values */
    static const seq_t init[22] = {
        1, 2, 4, 5, 6, 9, 10, 11, 15, 19, 21, 26,
        35, 48, 75, 120, 166, 203, 262, 395, 613, 879
    };
    /* shifts and corresponding coefficients */
    static const idx_t shift[8] = {15,16,17,18,19,20,21,22};
    static const seq_t coef[8] = {1,9,32,58,58,32,9,1};

    /* ---------  state  --------- */
    static seq_t *cache = NULL;   /* cache[0] = a(1)   */
    static idx_t cache_cap = 0;   /* allocated size    */
    static idx_t last_n = 0;      /* highest n cached  */

    /* ---------  initialise cache if necessary --------- */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* ---------  extend cache up to requested n --------- */
    for (idx_t step = last_n + 1; step <= n; ++step) {
        seq_t val;
        if (step <= 22) {
            val = init[step - 1];
        } else {
            /* recurrence */
            val = 0;
            for (int k = 0; k < 8; ++k) {
                val += coef[k] * cache[step - 1 - shift[k]];
            }
        }

        /* grow array if needed */
        if (step > cache_cap) {
            idx_t new_cap = cache_cap * 2;
            while (step > new_cap) new_cap *= 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            cache_cap = new_cap;
        }
        cache[step - 1] = val;
    }
    last_n = n;

    return cache[n - 1];
}
