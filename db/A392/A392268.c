/*
 * A392268  a(n) = Sum_{k=0..floor(2*n/5)} (k+1) * binomial(k,2*n-5*k).
 * https://oeis.org/A392268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(2*n/5)} (k+1) * binomial(k, 2*n-5*k)
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 80, u64 n <= 165,
 *            u128 n <= 338, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = (2 * n) / 5;   /* floor(2*n/5) */
    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t r = 2 * n - 5 * k;
        if (r > k) continue;   /* binomial(k, r) is zero when r>k */
        seq_t term = binomial(k, r);          /* k + 1 <= 2*n/5 +1 < 2*n/5+something */
        term = term * ((seq_t)k + 1);
        sum += term;
    }
    return sum;
}
