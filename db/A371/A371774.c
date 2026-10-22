/*
 * A371774  a(n) = Sum_{k=0..floor(n/3)} binomial(3*n-k+1,n-3*k).
 * https://oeis.org/A371774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(3*n-k+1, n-3*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;                  /* floor(n/3) */
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = 3 * n - k + 1;         /* upper argument of binomial */
        idx_t r = n - 3 * k;             /* lower argument of binomial */
        sum += binom(m, r);
    }
    return sum;
}
