/*
 * A249609  a(n) is the smallest m, 1<=m<=n, such that binomial(n,m) is evil
 *          (A001969); a(n)=0 if there is no such m.
 * https://oeis.org/A249609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest m (1 ≤ m ≤ n) such that binomial(n,m) is *
 *            evil (has an even number of 1's in its binary expansion). * a(n)
 *            = 0 if no such m exists
 * Range:     u8 n <= 13, u16 n <= 41, u32 n <= 75, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_evil
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_evil.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t bin = 1;                 /* C(n,0) */

    for (idx_t m = 1; m <= n; ++m) {
        bin = bin * (seq_t)(n - m + 1) / (seq_t)m;   /* C(n,m) */
        if (is_evil(bin)) return (seq_t)m;
    }
    return 0;                     /* no evil binomial found */
}
