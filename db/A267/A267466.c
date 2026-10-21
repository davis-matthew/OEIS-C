/*
 * A267466  Number of length-n 0..3 arrays with no following elements larger
 *          than the first repeated value.
 * https://oeis.org/A267466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-18 - 9*2^(1+n) + 50*3^n + 9*4^n + 4*3^n*n) / 36
 *            Recurrence: a(n) = 13*a(n-1) - 65*a(n-2) + 155*a(n-3) -
 *            174*a(n-4) + 72*a(n-5), with initial values a(1)=4, a(2)=16,
 *            a(3)=58, a(4)=204, a(5)=712
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is stored in 1‑based index semantics: cache[0] = a(1). */
    static seq_t *cache = NULL;   /* dynamic array of computed terms */
    static idx_t cap = 0;         /* number of terms currently stored         */

    /* Initial values for n = 1 … 5.  We only need these to seed the recurrence. */
    static const seq_t initial[5] = {4, 16, 58, 204, 712};

    /* Handle small n directly. */
    if (n <= 5)
        return initial[n - 1];

    /* Ensure the cache exists and contains at least the first 5 terms. */
    if (cap == 0) {
        size_t init_cap = 5;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        for (idx_t i = 0; i < init_cap; ++i)
            cache[i] = initial[i];
        cap = init_cap;
    }

    /* Extend the cache up to the requested n, computing new terms by the
       linear recurrence.  Since calls may come in any order, we compute
       all missing terms up to n on demand. */
    if (n > cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache)  /* allocation failed; nothing else we can do */
            return (seq_t)0;
        cache = new_cache;
        for (idx_t i = cap + 1; i <= n; ++i) {
            /* recurrence for i ≥ 6 */
            cache[i - 1] =
                13 * cache[i - 2]          /* a(i-1)   */
                - 65 * cache[i - 3]        /* a(i-2)   */
                + 155 * cache[i - 4]       /* a(i-3)   */
                - 174 * cache[i - 5]       /* a(i-4)   */
                + 72 * cache[i - 6];       /* a(i-5)   */
        }
        cap = n;
    }

    return cache[n - 1];
}
