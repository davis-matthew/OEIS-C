/*
 * A266699  Coefficient of x^2 in minimal polynomial of the continued fraction
 *          [1^n,1/2,1,1,1,...], where 1^n means n ones.
 * https://oeis.org/A266699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n)  :  A266699
 *
 * Recurrence: a(0)=4, a(1)=5, a(2)=1, a(3)=16
 *             a(n)=2*a(n-1)+2*a(n-2)-a(n-3)   for n>3
 *
 * This implementation uses lazy memoisation with a dynamic array.
 * It works for any integer type seq_t (including arbitrary‑precision
 * classes) and idx_t must be an unsigned 64‑bit integer type.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holds already computed terms */
    static seq_t *cache   = NULL;
    static idx_t  size    = 0;    /* number of stored terms (0 if cache empty) */
    static idx_t  capacity = 0;   /* allocated slots */

    /* enlarge the cache if we need more space */
    if (n + 1 > size) {
        if (capacity == 0) {
            capacity = 1;                 /* avoid shifting 0 */
        }
        while (capacity < n + 1) {
            capacity <<= 1;                /* double until big enough */
        }
        seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (!tmp) {
            return (seq_t)0;               /* out of memory – unlikely in tests */
        }
        cache = tmp;
    }

    /* compute missing values up to n (inclusive) */
    for (idx_t i = size; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 4;
        } else if (i == 1) {
            val = 5;
        } else if (i == 2) {
            val = 1;
        } else if (i == 3) {
            val = 16;
        } else {
            /* recurrence: 2*a(i-1) + 2*a(i-2) - a(i-3) */
            seq_t t = cache[i - 1] + cache[i - 2];
            t = t + t;                     /* 2*(a(i-1)+a(i-2)) */
            t = t - cache[i - 3];
            val = t;
        }
        cache[i] = val;
    }

    size = n + 1;
    return cache[n];
}
