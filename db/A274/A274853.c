/*
 * A274853  Number of n X 3 0..2 arrays with no element equal to any value at
 *          offset (-1,0) (0,-1) or (-2,-2) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) for n>4 with initial values
 *            a(1)=2, a(2)=9, a(3)=19, a(4)=43 The implementation memoizes
 *            a(n) in a static array so calls in any order are efficient
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static size_t buf_cap = 0;
    static idx_t max_computed = 0;

    if (n == 0)
        return 0;   /* unused, but safe */

    /* Ensure buffer capacity */
    if (buf_cap < (size_t)n + 1) {
        size_t new_cap = buf_cap == 0 ? 8 : buf_cap * 2;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        buf = tmp;
        buf_cap = new_cap;
    }

    /* Initialize base terms once */
    if (max_computed == 0) {
        buf[1] = (seq_t)2;
        buf[2] = (seq_t)9;
        buf[3] = (seq_t)19;
        buf[4] = (seq_t)43;
        max_computed = 4;
    }

    /* Return cached value if already computed */
    if (n <= max_computed)
        return buf[n];

    /* Compute terms from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        buf[i] = (seq_t)3 * buf[i-1] - (seq_t)2 * buf[i-2] + buf[i-3];
    }
    max_computed = n;
    return buf[n];
}
