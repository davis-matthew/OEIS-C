/*
 * A264414  G.f. A(x) satisfies: A(x)^2 = A(x^2) + 20*x.
 * https://oeis.org/A264414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 16, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A264414: a(0)=1, a(1)=10, a(2)=-45, ... computed from
   the functional equation A(x)^2 = A(x^2)+20*x.
   Recurrence: for n>=1,
   a(n) = ( [n==1?20:0] + (n%2==0? a(n/2):0) - sum_{i=1}^{n-1} a(i)a(n-i) ) / 2
   Using symmetry the sum is computed in half the time. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;           /* number of elements in cache */
    if (cap == 0) {                  /* first call: initialise cache */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 1;                /* a(0) = 1 */
        cap = 1;
    }

    /* extend cache up to index n if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache = new_cache;

        /* compute terms from cap to n inclusive */
        for (size_t k = cap; k <= (size_t)n; ++k) {
            seq_t rhs = 0;
            if (k == 1) rhs += 20;
            if ((k % 2) == 0) rhs += cache[k / 2];    /* a(k/2) */

            /* compute sum_{i=1}^{k-1} a(i)a(k-i) using symmetry */
            seq_t s = 0;
            size_t m = (k - 1) / 2;   /* floor((k-1)/2) */
            for (size_t i = 1; i <= m; ++i) {
                s += cache[i] * cache[k - i];
            }
            s *= 2;
            if ((k % 2) == 0) {
                s += cache[k / 2] * cache[k / 2];
            }

            seq_t two_a = rhs - s;            /* 2*a(k) */
            cache[k] = two_a / 2;             /* a(k) */
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
