/*
 * A286572  Compound filter (2-adic valuation of phi(n) & sigma(n)): a(n) =
 *          P(A053574(n), A000203(n)), where P(n,k) is sequence A000027 used
 *          as a pairing function.
 * https://oeis.org/A286572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * ( 2 + ((A053574(n) + A000203(n))^2) - A053574(n)
 *            - 3*A000203(n) )
 * Range:     u8 n <= 9, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A053574(n) is the 2‑adic valuation of phi(n)  */
/* and A000203(n) is the sum of divisors sigma(n).      */

#include <stdbool.h>
#include "../utils/phi.h"          /* seq_t phi(idx_t n); */
#include "../utils/sigma_of.h"     /* seq_t sigma_of(idx_t n); */

seq_t a(idx_t n)
{
    /* compute sigma(n) */
    seq_t sigma = sigma_of(n);

    /* compute phi(n) and its 2‑adic valuation */
    seq_t phi_n = phi(n);
    seq_t v2 = 0;
    while ((phi_n & 1) == 0) {
        v2 += 1;
        phi_n >>= 1;
    }

    /* compute (v2 + sigma)^2 */
    seq_t sum = v2 + sigma;
    seq_t sum_sq = sum * sum;

    /* apply the formula */
    seq_t result = sum_sq + (seq_t)2 - v2 - (seq_t)3 * sigma;

    /* divide by 2 (exact) */
    result >>= 1;

    return result;
}
