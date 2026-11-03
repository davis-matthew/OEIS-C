/*
 * A323816  Number of set-systems covering n vertices with no singletons.
 * https://oeis.org/A323816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t=0}^{n} (-1)^{n-t} * C(n,t) * 2^{(2^t - 1 - t)} (with
 *            2^t - 1 - t = 0 for t=0,1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 12 terms through n = 11 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t total = (seq_t)0;
    for (idx_t t = 0; t <= n; ++t) {
        seq_t pow2t = pow2(t);                 /* 2^t                            */
        seq_t L = pow2t - (seq_t)1 - (seq_t)t;            /* L(t) = 2^t - 1 - t              */
        if (t < 2) L = (seq_t)0;                         /* ensures non‑negative          */
        idx_t expL = (idx_t)L;                           /* fits into idx_t for small t    */
        seq_t powL = pow2(expL);              /* 2^{L(t)}                       */
        seq_t bin  = binomial(n, t);                /* C(n,t)                         */
        seq_t term = powL * bin;                         /* C(n,t) * 2^{L(t)}              */
        if (((n - t) & 1) != 0)                          /* sign is (-1)^{n-t}             */
            total = total - term;
        else
            total = total + term;
    }
    return total;
}
