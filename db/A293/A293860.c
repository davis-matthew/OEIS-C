/*
 * A293860  a(0) = 0, a(1) = 1; a(n) = n! * [x^n] exp(n*x) * Sum_{k=1..n-1}
 *          a(k)*x^k/k!.
 * https://oeis.org/A293860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} n^k * binomial(n,k) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0) = 0, a(1) = 1. */

#include <stdlib.h>

/* Note: in C++ the type seq_t is usually a class; the cast from
   void* is required.  In C the cast is optional.  The macro
   NDEBUG is not used here. */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t computed = 0;   /* highest index fully computed */

    /* Initialize cache for a(0) and a(1) on first call */
    if (!capacity) {
        capacity = 2;                         /* need indices 0 and 1 */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        computed = 1;
    }

    /* If n is already computed, return the stored value */
    if (n <= computed) {
        return cache[n];
    }

    /* Grow the cache to accommodate indices up to n */
    if (capacity <= n) {
        idx_t new_cap = capacity;
        while (new_cap <= n) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute values from computed+1 up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum   = (seq_t)0;
        seq_t power = (seq_t)1;   /* i^0 */
        seq_t binom = (seq_t)1;   /* C(i,0) */

        /* Sum over k = 1 .. i-1 */
        for (idx_t k = 1; k < i; ++k) {
            /* Update power: i^k */
            power = power * (seq_t)i;

            /* Update binomial: C(i,k) = C(i,k-1) * (i - k + 1) / k */
            binom = binom * (seq_t)(i - k + 1);
            binom = binom / (seq_t)k;

            /* term = i^k * binom(i,k) * a(i-k) */
            seq_t term = power * binom;
            term = term * cache[i - k];

            /* Accumulate */
            sum = sum + term;
        }
        cache[i] = sum;
    }
    computed = n;
    return cache[n];
}
