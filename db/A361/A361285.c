/*
 * A361285  Number of unordered triples of self-avoiding paths whose sets of
 *          nodes are disjoint subsets of a set of n points on a circle;
 *          one-node paths are allowed.
 * https://oeis.org/A361285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n-1)*(n-2)/384)*(7^(n-3) + 9*5^(n-3) + 3^n + 27)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 13, u64 n <= 24, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"


seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t n_seq = (seq_t)n;

    /* compute powers of the required bases */
    seq_t pow7 = big_pow((seq_t)7, (seq_t)(n_seq - 3));
    seq_t pow5 = big_pow((seq_t)5, (seq_t)(n_seq - 3));
    seq_t pow3 = big_pow((seq_t)3, n_seq);

    /* sum appearing in the formula */
    seq_t sum = pow7 + (seq_t)9 * pow5 + pow3 + (seq_t)27;

    /* factors to multiply: n, n-1, n-2, and the sum */
    seq_t factors[4] = { n_seq, n_seq - 1, n_seq - 2, sum };

    /* reduce each factor by the common divisor 384 to avoid overflow */
    seq_t denom = 384;
    for (size_t i = 0; i < 4 && denom > 1; ++i) {
        seq_t g = gcd(factors[i], denom);
        if (g > 1) {
            factors[i] /= g;
            denom     /= g;
        }
    }

    /* the product is now the desired value */
    seq_t result = 1;
    for (size_t i = 0; i < 4; ++i) {
        result *= factors[i];
    }
    return result;
}
