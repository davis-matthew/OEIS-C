/*
 * A289419  Number of Dyck paths of semilength n and height exactly 6.
 * https://oeis.org/A289419
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the linear recurrence a(n) = 11*a(n-1) - 46*a(n-2)
 *            + 91*a(n-3) - 86*a(n-4) + 34*a(n-5) - 4*a(n-6) with initial
 *            values a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=0, a(6)=1. This is derived
 *            from the denominator of the generating function of A289419
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array of a[0..cap-1] */
    static idx_t cap = 0;         /* current capacity of cache */
    static idx_t max_computed = 5;    /* highest index with a value known */

    /* values for n < 6 are zero */
    if (n < 6)
        return 0;

    /* Make sure cache can hold up to index n */
    if (cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)            /* allocation failed – return 0 as error */
            return 0;
        cache = tmp;
        /* initialize new entries to 0 */
        for (idx_t i = cap; i <= n; ++i)
            cache[i] = 0;
        cap = new_cap;
    }

    /* Ensure base values up to index 6 are set */
    if (max_computed < 6) {
        for (idx_t i = max_computed + 1; i <= 6; ++i)
            cache[i] = (i == 6) ? 1 : 0;
        max_computed = 6;
    }

    /* Compute missing terms up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t t1 = 11 * cache[k-1];
        seq_t t2 = 46 * cache[k-2];
        seq_t t3 = 91 * cache[k-3];
        seq_t t4 = 86 * cache[k-4];
        seq_t t5 = 34 * cache[k-5];
        seq_t t6 = 4  * cache[k-6];
        cache[k] = t1 - t2 + t3 - t4 + t5 - t6;
    }
    max_computed = n > max_computed ? n : max_computed;

    return cache[n];
}
