/*
 * A338857  With S(n,k) = Sum_{n<=j<=k} 1/(2*j+1), a(n)=k+1 such that S(n,k-1)
 *          < 1 <= S(n,k) for n>=0 and a(0)=1.
 * https://oeis.org/A338857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k : Σ_{j=n}^k 1/(2*j+1) ≥ 1} + 1 (offset 0)
 * Range:     u8 n <= 34, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Sum 1/(2*j+1) until it reaches at least 1.
       The loop variable j will stop at a(n), because after
       adding the term for j=k we increment j to k+1, which equals
       a(n).  Use a tiny epsilon to guard against rounding errors. */
    idx_t j = n;
    double sum = 0.0;
    const double eps = 1e-12;
    while (sum + eps < 1.0) {
        sum += 1.0 / (2.0 * j + 1.0);
        ++j;
    }
    return (seq_t)j;
}
