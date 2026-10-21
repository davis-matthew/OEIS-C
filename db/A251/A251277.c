/*
 * A251277  Number of (n+1) X (2+1) 0..3 arrays with no 2 X 2 subblock having
 *          its maximum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A251277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 54*a(n-1) - 182*a(n-2) + 228*a(n-3) + 1152*a(n-4), with
 *            initial terms: a(1)=3264, a(2)=164800, a(3)=8320896,
 *            a(4)=420152704
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;

    if (n == 0) {
        return (seq_t)0;
    }
    if (n > max_computed) {
        size_t needed = (size_t)(n + 1);   /* indices 0..n */
        if (!cache || needed > (size_t)(max_computed + 1)) {
            seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
            if (!tmp) {
                exit(1);
            }
            cache = tmp;
        }
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)3264;
            } else if (i == 2) {
                cache[i] = (seq_t)164800;
            } else if (i == 3) {
                cache[i] = (seq_t)8320896;
            } else if (i == 4) {
                cache[i] = (seq_t)420152704;
            } else {
                cache[i] = (seq_t)54 * cache[i - 1]
                           - (seq_t)182 * cache[i - 2]
                           + (seq_t)228 * cache[i - 3]
                           + (seq_t)1152 * cache[i - 4];
            }
        }
        max_computed = n;
    }
    return cache[n];
}
