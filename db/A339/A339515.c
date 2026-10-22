/*
 * A339515  a(n) = Sum_{k=0..floor(n/3)} k! * (n - 3*k)!.
 * https://oeis.org/A339515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} k! * (n - 3*k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   factorial_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, if needed by the build */
#include "../utils/factorial_value.h"

seq_t a(idx_t n)
{
    /* term_0 = 0! * n! = n! */
    seq_t fact = factorial_value(n);
    seq_t sum  = fact;
    seq_t term = fact;

    idx_t maxk = n / 3;          /* floor(n/3) */
    for (idx_t k = 1; k <= maxk; ++k)
    {
        idx_t m = n - 3 * (k - 1);       /* current m = n - 3*(k-1) */
        seq_t denom = (seq_t)m *
                      (seq_t)(m - 1) *
                      (seq_t)(m - 2);   /* m * (m-1) * (m-2) */

        term = term * (seq_t)k / denom;  /* term_k = term_{k-1} * k / denom */
        sum += term;
    }

    return sum;
}
