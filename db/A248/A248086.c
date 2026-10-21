/*
 * A248086  Sum of the eccentricities of all vertices in the Lucas cube
 *          Lambda(n).
 * https://oeis.org/A248086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*F(n+1) + (-1)^n*(n - floor(n/2))
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 39, u64 n <= 83, u128 n <= 174,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses fib(n) from utils/fib.h */
#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute n * Fibonacci(n+1) */
    seq_t fibn1 = fib(n + 1);
    seq_t result = (seq_t)n * fibn1;

    /* Compute the correction term: (-1)^n * ceil(n/2) */
    seq_t half     = (seq_t)(n / 2);       /* floor(n/2)   */
    seq_t ceil_half = (seq_t)((n + 1) / 2);/* ceil (n/2)   */

    if ((n & 1ULL) == 0) {
        /* Even n: add ceil(n/2) which equals n/2 */
        result += half;
    } else {
        /* Odd n: subtract ceil(n/2) which equals (n+1)/2 */
        result -= ceil_half;
    }

    return result;
}
