/*
 * A339572  If n even, a(n) = A000071(n/2+1); if n odd, a(n) =
 *          A001610((n-1)/2).
 * https://oeis.org/A339572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(k+2) + Fibonacci(k)*(n mod 2) - 1, where k =
 *            floor(n/2)
 * Range:     u8 n <= 22, u16 n <= 45, u32 n <= 91, u64 n <= 183,
 *            u128 n <= 368, big any n
 * Helpers:   fib_pair_iter
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/fib_pair_iter.h"

seq_t a(idx_t n)
{
    idx_t k = n >> 1;                /* floor(n/2)          */
    seq_t fk, fk1;
    fib_pair_iter(k, &fk, &fk1);     /* fk = F(k), fk1 = F(k+1) */
    seq_t fk2 = fk + fk1;            /* F(k+2) = F(k+1)+F(k) */
    seq_t result = fk2 - (seq_t)1;
    if ((n & 1U) != 0) {
        result += fk;               /* add F(k) if n is odd   */
    }
    return result;
}
