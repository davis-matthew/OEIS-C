/*
 * A360272  a(n) = Sum_{k=0..floor(n/4)} binomial(n-3*k,k) * Catalan(n-3*k).
 * https://oeis.org/A360272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binom(n-3*k,k) * catalan(n-3*k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = n / 4;                    /* floor(n/4) */
    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t m = n - 3 * k;               /* n - 3k */
        seq_t term = binomial(m, k);          /* binom(n-3k, k) */
        term = term * catalan(m);          /* * Catalan(n-3k) */
        sum += term;                       /* accumulate result */
    }
    return sum;
}
