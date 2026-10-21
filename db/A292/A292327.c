/*
 * A292327  p-INVERT of the Fibonacci sequence (A000045), where p(S) = (1 -
 *          S)^2.
 * https://oeis.org/A292327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) - a(n-4) for n>=5; a(0)=2,
 *            a(1)=5, a(2)=14, a(3)=38, a(4)=102
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t computed_until = 0;   /* first index not yet computed */

    /* Initial allocation and first five terms */
    if (!cache) {
        capacity = (n < 5) ? 5 : n + 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache)
            return (seq_t)0; /* allocation failure – unlikely in tests */

        cache[0] = (seq_t)2;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)38;
        cache[4] = (seq_t)102;
        computed_until = 5; /* indices 0..4 are ready */
    }

    if (n < computed_until)
        return cache[n];

    /* Ensure capacity for the requested index */
    if ((idx_t)(n + 1) > capacity) {
        idx_t new_cap = capacity * 2;
        if (new_cap <= n)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* allocation failure – unlikely in tests */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute terms up to n (inclusive) */
    for (idx_t k = computed_until; k <= n; ++k) {
        if (k < 5) continue;               /* already initialized */
        seq_t t1 = (seq_t)4 * cache[k - 1];
        seq_t t2 = (seq_t)2 * cache[k - 2];
        seq_t t3 = (seq_t)4 * cache[k - 3];
        seq_t t4 = cache[k - 4];
        cache[k] = t1 - (t2 + t3 + t4);
    }

    computed_until = n + 1;
    return cache[n];
}
