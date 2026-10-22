/*
 * A377824  Sum of the positions of minimum parts in all compositions of n.
 * https://oeis.org/A377824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} (k*(k+1)/2) * sum_{j=0..⌊n/k⌋-1} binom(n -
 *            k*j - 2, k-2), with a(0)=0, a(1)=1. The binom function is taken
 *            from the utils library
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 27, u64 n <= 57, u128 n <= 119,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;

    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (k == 1) {
            total += (seq_t)1;   // only one composition (n)
            continue;
        }
        idx_t q = n / k;          // number of possible minimal parts m
        if (q == 0)
            continue;             // no compositions for this k

        seq_t factor = (seq_t)k * (k + 1) / 2;   // k*(k+1)/2
        seq_t inner  = 0;

        for (idx_t j = 0; j < q; ++j) {
            idx_t t = n - k * j - 2;           // argument of binomial
            inner += binom(t, k - 2);
        }

        total += factor * inner;
    }
    return total;
}
