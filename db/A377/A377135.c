/*
 * A377135  Number of maximal chains in the poset of n-ary words of length n
 *          ordered by B covers A iff A_i <= B_i for 1 <= i <= n.
 * https://oeis.org/A377135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n-1))! / ((n-1)!^n)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t result = (seq_t)1;
    idx_t k = n - 1;                     /* k = n-1 */
    for (idx_t i = 2; i <= n; ++i) {     /* product over i = 2 .. n */
        idx_t N = k * i;                 /* N = (n-1)*i */
        seq_t cur = (seq_t)1;
        for (idx_t j = 1; j <= k; ++j) { /* compute binom(N, k) */
            cur = cur * (seq_t)(N - j + 1);
            cur = cur / (seq_t)j;
        }
        result = result * cur;
    }
    return result;
}
