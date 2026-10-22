/*
 * A318200  Expansion of Hypergeometric function F(17/12, 13/12; 3; 1728*x) in
 *          powers of x.
 * https://oeis.org/A318200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*12^n/(n!*(n+2)!)) * ∏_{k=0}^{n-1}(12k+17)(12k+13)
 *            Recurrence (valid for all n ≥ 0): a(0) = 1 a(n+1) = a(n) * 12 *
 *            (12n+17) * (12n+13) / ((n+1)(n+3)) (The division is exact, so
 *            the intermediate result is an integer.)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] = a(i) for i ≤ last */
    static seq_t *cache = NULL;
    static idx_t last = 0;

    /* First call: allocate cache for at least n+1 entries */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cache[0] = (seq_t)1;        /* a(0) = 1 */
        last   = 0;
    }

    /* Extend cache if n is beyond current size */
    if (n > last) {
        /* Resize the array to hold n+1 elements */
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        /* Compute missing terms using the recurrence */
        for (idx_t i = last + 1; i <= n; ++i) {
            /* i = current index, k = i-1 is the previous n in the recurrence */
            idx_t k = i - 1;

            seq_t prev = cache[i - 1];
            seq_t tmp  = prev * (seq_t)12;               /* multiply by 12 */
            tmp       /= (seq_t)(k + 1);                 /* divide by (n+1) */
            tmp       *= (seq_t)(12 * k + 17);           /* multiply by (12n+17) */
            tmp       /= (seq_t)(k + 3);                 /* divide by (n+3) */
            tmp       *= (seq_t)(12 * k + 13);           /* multiply by (12n+13) */

            cache[i] = tmp;
        }
        last = n;
    }

    return cache[n];
}
