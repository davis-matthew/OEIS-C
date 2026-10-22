/*
 * A390781  a(n) = Sum_{k=0..floor(n/3)} (k+1) * 2^k * 3^(n-3*k) *
 *          binomial(2*k,2*(n-3*k)).
 * https://oeis.org/A390781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} (k+1) · 2^k · 3^{\,n-3k} · binomial(2k,
 *            2(n-3k))
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 28, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t max_k = n / 3;          /* floor(n/3) */
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t n_minus = n - 3 * k;                 /* n − 3k */
        seq_t term = (seq_t)(k + 1);                /* (k+1)   */
        term *= pow2(k);                           /* × 2^k   */
        term *= pow3(n_minus);                    /* × 3^{n-3k} */
        term *= binomial(2 * k, 2 * n_minus);      /* × binomial(2k,2(n-3k)) */
        result += term;
    }
    return result;
}
