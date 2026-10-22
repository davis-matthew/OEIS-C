/*
 * A348060  a(n) = Sum_{k=1..n, gcd(n,k) = 1} (k-1) / gcd(n,k-1).
 * https://oeis.org/A348060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n, gcd(n,k)=1} (k-1)/gcd(n,k-1)
 * Range:     u8 n <= 28, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* only include terms where k is coprime to n */
        if (gcd((seq_t)n, (seq_t)k) == 1) {
            seq_t denom = gcd((seq_t)n, (seq_t)(k - 1));
            seq_t term   = ((seq_t)(k - 1)) / denom;
            total += term;
        }
    }
    return total;
}
