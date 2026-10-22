/*
 * A354401  a(n) is the denominator of Sum_{k=1..n} 1 / (k*k!).
 * https://oeis.org/A354401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of ∑_{k=1..n} 1/(k*k!)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   my_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/my_gcd.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;    /* cache[i-1] holds a(i) */
    static size_t cache_cap = 0;   /* allocated size of cache   */
    static idx_t cur_n = 0;        /* largest n processed       */
    static seq_t cur_num = 0;      /* numerator of the running sum */
    static seq_t cur_den = 1;      /* denominator of the running sum */
    static seq_t fact = 1;         /* current factorial (cur_n !)   */

    if (n == 0) return 0; /* below offset – value irrelevant */

    /* enlarge cache if required */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* return cached value if already computed */
    if (n <= cur_n)
        return cache[n - 1];

    /* extend computation up to n */
    for (idx_t i = cur_n + 1; i <= n; ++i) {
        fact *= (seq_t)i;                     /* i! = (i-1)! * i */
        seq_t denom_i = (seq_t)i * fact;      /* denominator of new summand */

        seq_t g1 = my_gcd(cur_den, denom_i);  /* gcd for lcm calculation */
        seq_t d1 = cur_den / g1;
        seq_t d2 = denom_i / g1;

        /* new fraction after adding 1/denom_i */
        seq_t new_num = cur_num * d2 + d1;
        seq_t new_den = d1 * denom_i;

        /* reduce the fraction */
        seq_t g2 = my_gcd(new_num, new_den);
        new_num /= g2;
        new_den /= g2;

        /* store denominator of the partial sum */
        cache[i - 1] = new_den;

        /* update running sum for next iteration */
        cur_num = new_num;
        cur_den = new_den;
    }

    cur_n = n;
    return cache[n - 1];
}
