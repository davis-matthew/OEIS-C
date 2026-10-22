/*
 * A387690  a(n) = Sum_{k=0..floor(n/3)} 2^(n-3*k) * binomial(2*n-4*k,2*k).
 * https://oeis.org/A387690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2^(n-3*k) * binomial(2*n-4*k, 2*k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility functions pow2(idx_t) and binomial(idx_t,idx_t) */
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t p = pow2(n - 3 * k);
        idx_t m = 2 * n - 4 * k;   /* upper argument of the binomial */
        idx_t r = 2 * k;           /* lower argument of the binomial */
        seq_t c = binomial(m, r);
        sum += p * c;
    }
    return sum;
}
