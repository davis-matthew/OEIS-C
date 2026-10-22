/*
 * A368981  a(n) = Sum_{k=0..n} binomial(n, k - 1)*(1 - k)^(k - 1)*(n - k)*(n
 *          - k + 1)^(n - k).
 * https://oeis.org/A368981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n, k - 1)*(1 - k)^(k - 1)*(n -
 *            k)*(n - k + 1)^(n - k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* positive and negative parts of the sum */
    seq_t possum = 0;
    seq_t negsum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t r = k - 1;                      /* exponent of (1-k) */
        seq_t bin = binomial(n, r);          /* binomial coefficient */

        /* magnitude of (1-k)^(k-1) = (k-1)^(k-1) */
        seq_t pow_abs;
        if (r == 0) {
            pow_abs = 1;                     /* 0^0 is treated as 1 */
        } else {
            pow_abs = big_pow((seq_t)(k - 1), (seq_t)r);
        }

        idx_t nmk = n - k;                    /* n - k */
        seq_t pow2 = big_pow((seq_t)(nmk + 1), (seq_t)nmk); /* (n-k+1)^(n-k) */

        /* term magnitude */
        seq_t term = bin;
        term *= pow_abs;
        term *= (seq_t)nmk;
        term *= pow2;

        /* sign determined by the parity of r = k-1 */
        if (r & 1) {
            negsum += term;  /* odd exponent ⇒ negative sign */
        } else {
            possum += term;  /* even exponent or r == 0 ⇒ non‑negative sign */
        }
    }

    /* possum ≥ negsum for every n, so unsigned subtraction is safe */
    return possum - negsum;
}
