/*
 * A367927  a(n) = Product_{k=1..n} (n^2 + k^2)^k.
 * https://oeis.org/A367927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (n^2 + k^2)^k
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                      /* a(0) = 1 */

    /* Compute n^2 once and reuse */
    seq_t nsq = (seq_t)n * (seq_t)n;

    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t ksq = (seq_t)k * (seq_t)k;
        seq_t base = nsq + ksq;
        seq_t term = big_pow(base, (seq_t)k);
        res *= term;
    }
    return res;
}
