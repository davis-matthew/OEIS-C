/*
 * A260794  Number of steps required by R. L. Graham's generalized binary
 *          merging algorithm.
 * https://oeis.org/A260794
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(3) = 2, a(n) = a(n-1) + 2^k for n ≥ 4, where k is the largest
 *            integer such that k(k+1)/2 ≤ n-4
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n) {
    if (n < 3)
        return 0;          /* value for indices below the offset is undefined */
    if (n == 3)
        return (seq_t)2;

    seq_t result = (seq_t)2;          /* a(3) */
    for (idx_t i = 4; i <= n; ++i) {
        idx_t t = i - 4;              /* t = n - 4 as in the definition */
        /* Find largest k with k(k+1)/2 <= t */
        idx_t k = 0;
        while ((k + 1) * (k + 2) / 2 <= t)
            ++k;                     /* increment until condition fails */
        seq_t diff = (seq_t)1 << k;   /* 2^k */
        result += diff;
    }
    return result;
}
