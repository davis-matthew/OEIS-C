/*
 * A275064  Number of permutations p of [n] such that p(i)-i is a multiple of
 *          nine for all i in [n].
 * https://oeis.org/A275064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=0..8} floor((n+i)/9)!
 * Range:     u8 n <= 16, u16 n <= 22, u32 n <= 31, u64 n <= 45, u128 n <= 68,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 0; i <= 8; ++i) {
        idx_t k = (n + i) / 9;          /* floor((n+i)/9) */
        seq_t f = factorial(k);         /* f = k! */
        result *= f;
    }
    return result;
}
