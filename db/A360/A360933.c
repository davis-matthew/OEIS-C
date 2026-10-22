/*
 * A360933  Expansion of e.g.f. Sum_{k>=0} exp((3^k - 1)*x) * x^k/k!.
 * https://oeis.org/A360933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (3^k - 1)^(n-k) * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   binom, pow3, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/binom.h"    /* provides binom(idx_t a, idx_t b) */
#include "../utils/pow3.h"     /* provides pow3(idx_t exp) */
#include "../utils/seq_pow.h"  /* provides seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* base = 3^k - 1 */
        seq_t base = pow3(k);
        base -= (seq_t)1;

        /* pow_part = (3^k - 1)^(n-k) */
        seq_t pow_part = seq_pow(base, n - k);

        /* bin = binomial(n, k) */
        seq_t bin = binom(n, k);

        /* add term to total */
        total += pow_part * bin;
    }
    return total;
}
