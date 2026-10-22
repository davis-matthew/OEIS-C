/*
 * A358081  Expansion of e.g.f. 1/(1 - x^3 * exp(x)).
 * https://oeis.org/A358081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} k^(n-3*k)/(n-3*k)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t max_k = n / 3;
    seq_t sum = 0;
    seq_t comb = 1;          /* comb = product_{i=0}^{3k-1} (n - i)  */
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t rem = n - 3 * k;            /* exponent for k */
        seq_t powk;

        if (k == 0) {
            /* 0^0 = 1, 0^positive = 0 */
            powk = (rem == 0) ? (seq_t)1 : (seq_t)0;
        } else {
            powk = (rem == 0) ? (seq_t)1 : big_pow((seq_t)k, (seq_t)rem);
        }

        if (powk != (seq_t)0) {
            seq_t term = comb * powk;
            sum += term;
        }

        if (k == max_k) break;   /* no next k */

        /* update comb for next k: multiply by (n-3k)(n-3k-1)(n-3k-2) */
        seq_t f1 = (seq_t)(n - 3 * k);
        seq_t f2 = (seq_t)(n - 3 * k - 1);
        seq_t f3 = (seq_t)(n - 3 * k - 2);
        comb = comb * f1 * f2 * f3;
    }
    return sum;
}
