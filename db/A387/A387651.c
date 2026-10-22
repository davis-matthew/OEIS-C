/*
 * A387651  a(n) = Sum_{k=0..floor(n/2)} 2^(n-2*k) *
 *          binomial(2*k+1,2*n-4*k+1).
 * https://oeis.org/A387651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} 2^(n-2*k) * binomial(2*k+1,
 *            2*n-4*k+1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* start index where the binomial is non‑zero: 2*n - 4*k + 1 <= 2*k + 1
       which gives k >= ceil(n/3). */
    idx_t low  = (n + 2) / 3;      /* ceil(n/3) */
    idx_t high = n / 2;            /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t k = low; k <= high; ++k) {
        idx_t m = 2 * k + 1;                     /* upper side of binomial */
        idx_t s = 2 * n - 4 * k + 1;             /* lower side of binomial */

        /* The binomial term is zero if s > m, but the loop bounds already
           guarantee this does not happen, so we can proceed safely. */
        seq_t term = pow2(n - 2 * k) * binomial(m, s);
        sum += term;
    }
    return sum;
}
