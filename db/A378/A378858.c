/*
 * A378858  G.f. A(x) satisfies A(x) = ( 1 + x/(1 - x*A(x)^(3/4)) )^4.
 * https://oeis.org/A378858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * Σ_{k=0..n} ( binomial(3(n‑k)+4, k) * binomial(n‑1,
 *            n‑k) ) / (3(n‑k)+4) with a(0) = 1 This follows from the
 *            generating‑function formula for A378858
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t sub   = n - k;                /* n‑k  */
        idx_t denom = 3 * sub + 4;          /* 3(n‑k)+4  */

        seq_t bin1 = binomial(denom, k);          /* binomial(3(n‑k)+4, k)  */
        seq_t bin2 = binomial(n - 1, sub);        /* binomial(n‑1, n‑k)     */

        seq_t prod = seq_mul(bin1, bin2);         /* bin1 * bin2            */
        prod = seq_mul(prod, (seq_t)4);             /* multiply by r = 4      */
        prod = prod / (seq_t)denom;               /* divide by (3(n‑k)+4)    */

        sum += prod;
    }
    return sum;
}
