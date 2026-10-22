/*
 * A368510  a(n) = Sum_{k=0..n} n^(n-k) * binomial(k+n,k).
 * https://oeis.org/A368510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} n^(n-k) * binomial(k+n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    /* compute n^n once */
    seq_t power = pow_uint((uint64_t)n, n);
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* C(k+n, k) = C(k+n, n) */
        seq_t bin = binomial(k + n, k);
        result += power * bin;
        if (k != n)          /* divide by n for next term */
            power /= (seq_t)n;
    }
    return result;
}
