/*
 * A375470  a(n) = Sum_{k=0..floor(n/3)} (k+1) * binomial(k,n-3*k)^2.
 * https://oeis.org/A375470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (k+1) * binomial(k, n-3*k)^2
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t r = n - 3 * k;          /* n - 3k is non‑negative */
        seq_t c = binomial(k, r);      /* C(k, r) */
        seq_t term = (seq_t)(k + 1);
        term *= c;                    /* (k+1) * C(k, r) */
        term *= c;                    /* * C(k, r) again */
        sum += term;
    }
    return sum;
}
