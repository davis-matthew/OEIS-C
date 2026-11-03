/*
 * A360743  Number of idempotent binary relation matrices E on [n] such that E
 *          contains an identity matrix of order n-1 and (E - I_n)^2 = 0.
 * https://oeis.org/A360743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 1) * sum_{k=0}^{n} binom(n,k) * (2^k - 1)^{n-k} *
 *            where binom is the binomial coefficient and the exponentiation
 *            is exact. * * Implementation uses the combinatorial expansion
 *            derived from the EGF: * A_n = Σ_{k=0}^n C(n,k) * (2^k - 1)^{n-k}
 *            * a(n) = (n+1) * A_n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 101 (2026-11-01)
 * Helpers:   binomial, ipow_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin = binomial(n, k);
        seq_t base = pow2(k) - 1;
        idx_t exp = n - k;
        seq_t pow_val;
        if (exp == 0) {
            pow_val = 1;
        } else if (k == 0) {          /* base == 0 */
            pow_val = 0;
        } else {
            pow_val = ipow_idx(base, exp);
        }
        sum += bin * pow_val;
    }
    seq_t result = sum * ((seq_t)(n + 1));
    return result;
}
