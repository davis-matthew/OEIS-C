/*
 * A377150  a(n) = Sum_{k=0..floor(n/3)} binomial(k+3,3) *
 *          binomial(k,n-3*k)^2.
 * https://oeis.org/A377150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} binomial(k+3,3) * binomial(k,n-3k)^2
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 47, u64 n <= 98, u128 n <= 205,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t kmax = n / 3;
    seq_t sum = 0;

    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t c1 = binomial(k + 3, 3);          /* C(k+3,3)          */
        idx_t m  = n - 3 * k;                   /* n-3k              */
        if (m <= k) {                           /* binomial(k,m) non‑zero  */
            seq_t c2 = binomial(k, m);          /* C(k,n-3k)          */
            seq_t tmp = c2 * c2;                /* square c2           */
            sum += c1 * tmp;                    /* accumulate          */
        }
    }
    return sum;
}
