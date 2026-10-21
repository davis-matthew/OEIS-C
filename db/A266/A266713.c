/*
 * A266713  Coefficient of x^3 in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(2),1,1,...], where 1^n means n ones.
 * https://oeis.org/A266713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*a(n-1)+15*a(n-2)-15*a(n-3)-5*a(n-4)+a(n-5)+N_n where
 *            N_n = 2,-8,-42,-44,114,-40,-24,4 for n = 0…7, and N_n = 0 for n
 *            ≥ 8. Recurrence holds for all n ≥ 5. This implements the
 *            recurrence from the generating function of A266713
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 23, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t N[8] = {
    (seq_t)  2,  /* N_0 */
    (seq_t) -8,  /* N_1 */
    (seq_t)-42,  /* N_2 */
    (seq_t)-44,  /* N_3 */
    (seq_t) 114, /* N_4 */
    (seq_t)-40,  /* N_5 */
    (seq_t)-24,  /* N_6 */
    (seq_t)   4  /* N_7 */
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = (idx_t)-1;   /* -1 means nothing computed yet */

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough. */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – return 0 */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values in order. */
    size_t start = 0;
    if (last_computed != (idx_t)-1) start = (size_t)last_computed + 1;
    for (size_t i = start; i <= (size_t)n; ++i) {
        if (i == 0)           cache[0] = (seq_t) 2;
        else if (i == 1)      cache[1] = (seq_t) 2;
        else if (i == 2)      cache[2] = (seq_t)-2;
        else if (i == 3)      cache[3] = (seq_t)-54;
        else if (i == 4)      cache[4] = (seq_t)-226;
        else {
            seq_t val = cache[i-1] * (seq_t)5
                     + cache[i-2] * (seq_t)15
                     - cache[i-3] * (seq_t)15
                     - cache[i-4] * (seq_t)5
                     + cache[i-5];
            if (i <= 7) val += N[i];
            cache[i] = val;
        }
    }
    last_computed = n;
    return cache[(size_t)n];
}
