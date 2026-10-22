/*
 * A351118  a(n) is the number of endofunctions on an n-set where there is an
 *          element with a preimage of cardinality greater than n/2.
 * https://oeis.org/A351118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{k=0..floor((n-1)/2)} binomial(n,k)*(n-1)^k
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 36,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined for n >= 1; return 0 for safety. */
        return 0;
    }

    idx_t maxk = (n - 1) / 2;         /* floor((n-1)/2) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t bin = binomial(n, k);                     /* C(n,k) */
        seq_t pw  = pow_uint((uint64_t)(n - 1), k);      /* (n-1)^k */
        sum += bin * pw;                                 /* add term */
    }

    return (seq_t)n * sum;                                 /* n * sum */
}
