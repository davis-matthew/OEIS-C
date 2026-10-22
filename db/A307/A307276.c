/*
 * A307276  Partial sums of A307270.
 * https://oeis.org/A307276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-4) - 2*a(n-5) - a(n-8) + a(n-9) with
 *            a(0)=1, a(1)=4, a(2)=10, a(3)=19, a(4)=32, a(5)=50, a(6)=77,
 *            a(7)=108, a(8)=142, a(9)=183, a(10)=233, …
 * Range:     u8 n <= 10, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization array */
    static seq_t *memo = NULL;
    static size_t capacity = 0;
    static idx_t  last_computed = 0;

    /* initial terms (first 18) */
    static const seq_t init[18] = {
        1, 4, 10, 19, 32, 50, 77, 108,
        142, 183, 233, 291, 352, 416, 491, 572, 654, 739
    };

    /* create memo array if needed and store a(0) */
    if (!memo) {
        capacity = 1;
        memo = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!memo) abort();
        memo[0] = init[0];
        last_computed = 0;
    }

    /* enlarge the array if n is outside the current capacity */
    if (capacity <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        memo = tmp;
        capacity = new_cap;
    }

    /* compute missing values up to n */
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i < 18) {
                memo[i] = init[(size_t)i];
            } else {
                /* recurrence: a(n) = a(n-1) + 2*a(n-4)
                                 - 2*a(n-5) - a(n-8) + a(n-9) */
                seq_t val = memo[i - 1];
                val += memo[i - 4] * (seq_t)2;
                val -= memo[i - 5] * (seq_t)2;
                val -= memo[i - 8];
                val += memo[i - 9];
                memo[i] = val;
            }
        }
        last_computed = n;
    }

    return memo[n];
}
