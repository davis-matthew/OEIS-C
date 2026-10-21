/*
 * A282237  a(n) = Fibonacci(n) represented in bijective base-6 numeration.
 * https://oeis.org/A282237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A057436(A000045(n))
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 74, u128 n <= 145,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = bijective_base6(Fibonacci(n)) */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f     = fib(n);            /* Fibonacci(n)  */
    seq_t res   = 0;                 /* result in bijective base-6 (decimal) */
    seq_t pow10 = 1;                 /* 10^k for building decimal number */

    while (f > 0) {
        seq_t r = f % (seq_t)6;      /* remainder in 0..5 */
        if (r == 0) {
            r = (seq_t)6;
            f = f / (seq_t)6 - 1;    /* adjust for bijective base */
        } else {
            f = f / (seq_t)6;
        }
        res += r * pow10;
        pow10 *= (seq_t)10;
    }
    return res;
}
