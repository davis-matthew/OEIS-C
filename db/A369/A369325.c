/*
 * A369325  a(n) is the number of words of length n over the alphabet [n],
 *          avoiding 120 and 210, and sortable by a stack of depth 2.
 * https://oeis.org/A369325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n % 2) + 2^n * Sum_{i=0..floor((n-2)/2)} binomial(2*(n -
 *            i)- 3, n - 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Base cases: for n = 0 and 1 the sum is empty. */
    if (n < 2)
        return (seq_t)(n % 2);

    seq_t sum = 0;
    idx_t max_i = (n - 2) / 2;          /* floor((n-2)/2) */
    idx_t k = n - 1;                    /* constant for all terms */

    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t a = 2 * (n - i) - 3;      /* upper part of the binomial */
        seq_t term = binom(a, k);
        sum += term;                    /* accumulate using seq_t addition */
    }

    seq_t result = pow2(n) * sum;       /* 2^n * sum */
    if (n & 1)                          /* add 1 for odd n */
        result += 1;

    return result;
}
