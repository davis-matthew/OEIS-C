/*
 * A333463  a(n) = Sum_{k=1..n} floor(n/k) * gcd(n,k).
 * https://oeis.org/A333463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k) * gcd(n,k)
 * Range:     u8 n <= 29, u16 n <= 1199, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t n_idx = n;
    seq_t res = 0;
    for (idx_t k = 1; k <= n_idx; ++k) {
        idx_t div = n_idx / k;              /* floor(n/k) */
        seq_t g = gcd((seq_t)n_idx, (seq_t)k);  /* gcd(n,k) */
        /* The product div * g fits in idx_t for all n for which the
           result fits in seq_t; otherwise the overflow is consistent
           with the limited width of seq_t. */
        res += (seq_t)(div * (idx_t)g);
    }
    return res;
}
