/*
 * A298225  Number of nX3 0..1 arrays with every element equal to 0, 1, 3, 4
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + 13*a(n-3) - 14*a(n-4) - 29*a(n-5) -
 *            23*a(n-6) + 29*a(n-7) + 56*a(n-8) - 30*a(n-9) - 7*a(n-10) +
 *            41*a(n-11) + 2*a(n-12) - 26*a(n-13) - 12*a(n-14) - 15*a(n-15) -
 *            7*a(n-16) + 16*a(n-17) + 2*a(n-18) (for n > 19) Initial values:
 *            a(1)=3, a(2)=4, a(3)=5, a(4)=17, a(5)=12, a(6)...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Respect offset 1: for n <= 0 return 0 */
    if (n == 0)
        return 0;

    /* Initial terms a(1) … a(19) */
    static const seq_t init[19] = {
        (seq_t)3, (seq_t)4, (seq_t)5, (seq_t)17,
        (seq_t)12, (seq_t)100, (seq_t)219, (seq_t)498,
        (seq_t)1999, (seq_t)4953, (seq_t)13928, (seq_t)45164,
        (seq_t)120865, (seq_t)355474, (seq_t)1065723, (seq_t)2987665,
        (seq_t)8792232, (seq_t)25703174, (seq_t)73648001
    };

    /* Coefficients for a(n-1) … a(n-18) */
    static const int coeff[18] = {
        1, 4, 13, -14, -29, -23, 29, 56, -30, -7,
        41, 2, -26, -12, -15, -7, 16, 2
    };

    /* Cache for memoisation (1‑based index) */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0; /* number of filled terms */
    static size_t cache_cap  = 0; /* allocated capacity */

    /* Target index (n is 1‑based) */
    size_t target = (size_t)n;

    /* Return if already computed */
    if (target <= cache_size)
        return cache[target - 1];

    /* Ensure capacity */
    if (target > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap < target)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)      /* out of memory – return 0 */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to target */
    for (size_t k = cache_size + 1; k <= target; ++k) {
        seq_t value;

        if (k <= 19) {                     /* use initial data */
            value = init[k - 1];
        } else {                           /* recurrence for k > 19 */
            seq_t sum = 0;
            for (int i = 0; i < 18; ++i) {
                const seq_t term = cache[k - 2 - i]; /* a(k-1-i) */
                if (coeff[i] >= 0)
                    sum += term * (seq_t)coeff[i];
                else
                    sum -= term * (seq_t)(-coeff[i]);
            }
            value = sum;
        }

        cache[k - 1] = value;
    }

    cache_size = target;
    return cache[target - 1];
}
