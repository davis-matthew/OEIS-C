/*
 * A345129  Sum of the squarefree products s*t from all positive integer pairs
 *          (s,t), such that s + t = n, s <= t.
 * https://oeis.org/A345129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} k * (n-k) * mu(k*(n-k))^2
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h" /* function seq_t mobius(idx_t m) */

/* seq_t a(idx_t n)
 * Computes the sum of the squarefree products of all unordered
 * positive integer pairs (s,t) with s+t = n, s <= t.
 */
seq_t a(idx_t n) {
    if (n <= 1)
        return 0;                 /* a(1) = 0  */

    idx_t half = n / 2;           /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t k = 1; k <= half; ++k) {
        idx_t t = n - k;
        idx_t prod = k * t;       /* fits in idx_t for n <= 10000 */

        seq_t mu  = mobius(prod);          /* μ(prod) may be -1,0,1 */
        seq_t mu2 = (mu != 0) ? (seq_t)1 : (seq_t)0; /* μ^2 */

        seq_t term = (seq_t)k * (seq_t)t * mu2;
        sum += term;
    }

    return sum;
}
