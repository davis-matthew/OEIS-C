/*
 * A257388  Number of 4-Motzkin paths of length n with no level steps at odd
 *          level.
 * https://oeis.org/A257388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..floor(n/2)} 4^(n-2i) * C(i) * binomial(n-i,i)
 *            where C(i) is the i‑th Catalan number
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   binomial, catalan, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/catalan.h"
#include "../utils/binomial.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n / 2;
    for (idx_t i = 0; i <= half; ++i) {
        /* Catalan number C(i) */
        seq_t term = catalan(i);

        /* binomial coefficient binomial(n-i, i) */
        term *= binomial(n - i, i);

        /* power of four: 4^(n-2i) */
        idx_t exp = n - 2 * i;
        term *= pow4(exp);

        sum += term;
    }
    return sum;
}
