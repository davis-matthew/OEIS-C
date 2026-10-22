/*
 * A307972  G.f. A(x) satisfies: A(x) = 1 + x + x^2 + x^3 + x^4 + x^5*A(x)^2.
 * https://oeis.org/A307972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 33, u32 n <= 59, u64 n <= 110,
 *            u128 n <= 209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=a(2)=a(3)=a(4)=1;
   For n>=0, a(n+5)=∑_{k=0..n} a(k)*a(n-k) */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *data = NULL;
    static size_t cap   = 0;          /* allocated array size            */
    static idx_t  last = (idx_t)-1;   /* highest computed index = -1 initially */

    /* Grow the cache so it can hold indices up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (cap < 16) ? 16 : cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
        data = tmp;
        cap  = new_cap;
    }

    /* Initialise the first five terms if needed */
    for (idx_t i = 0; i <= 4 && i <= n; ++i)
        data[i] = (seq_t)1;

    if (n == last)      /* already computed */
        return data[n];

    /* Compute new terms only (skip already computed indices) */
    idx_t start = (last + 1 > 5) ? last + 1 : 5;
    for (idx_t i = start; i <= n; ++i) {
        idx_t m   = i - 5;          /* index in the recurrence */
        seq_t sum = 0;
        for (idx_t k = 0; k <= m; ++k)
            sum += data[k] * data[m - k];
        data[i] = sum;
    }
    last = n;
    return data[n];
}
