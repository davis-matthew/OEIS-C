/*
 * A387510  a(n) = Sum_{k=0..floor(n/3)} 2^(n-2*k) * binomial(n-2*k,k)^2.
 * https://oeis.org/A387510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2^(n-2*k) * binomial(n-2*k,k)^2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n / 3;
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t m = n - 2 * k;
        seq_t pow = pow2(m);          /* 2^(n-2*k) */
        seq_t c   = binomial(m, k);   /* binomial(n-2*k,k) */
        seq_t term = pow * c * c;     /* square of binomial */
        sum += term;
    }
    return sum;
}
