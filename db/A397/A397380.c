/*
 * A397380  a(n) = Sum_{k=0..n} binomial(n+k,2*k) * (binomial(n+2*k,k) +
 *          binomial(n+2*k,k-1)).
 * https://oeis.org/A397380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k,2*k) *
 *            (binomial(n+2*k,k)+binomial(n+2*k,k-1))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin1 = binomial(n + k, 2 * k);          /* binomial(n+k, 2*k) */
        seq_t bin2 = binomial(n + 2 * k, k);           /* binomial(n+2*k, k)  */
        seq_t bin3 = 0;
        if (k > 0) bin3 = binomial(n + 2 * k, k - 1);  /* binomial(n+2*k, k-1) */
        seq_t inner = bin2 + bin3;
        total += bin1 * inner;
    }
    return total;
}
