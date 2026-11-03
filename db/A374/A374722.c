/*
 * A374722  Number of nonisomorphic spanning trees of the nC_5-snake graph
 *          with constant distance between cutpoints.
 * https://oeis.org/A374722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/2)*(3*5^(n-2) + 5^floor((n-2)/2)) for n>1, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 443 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    seq_t k = (seq_t)(n - 2);
    seq_t pow1 = ipow((seq_t)5, k);
    seq_t pow2 = ipow((seq_t)5, k / 2);
    seq_t sum  = pow1 * 3 + pow2;
    sum = sum * 3;
    return sum / 2;
}
