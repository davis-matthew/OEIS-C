/*
 * A294322  a(n) = Product_{k=0..n} (4*k + 3)!.
 * https://oeis.org/A294322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (4*k + 3)!
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Handle the case n = 0 */
    if (n == 0)
        return factorial(3);

    seq_t product = factorial(3);          /* a(0) */
    seq_t fact    = product;               /* current factorial (4*k+3)! */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* The next factorial is fact * (prev+1)*(prev+2)*(prev+3)*(prev+4)  */
        /* where prev = 4*(k-1) + 3 = 4*k - 1 */
        seq_t prev = 4 * k - 1;            /* prev = 4*k - 1 was 4*(k-1)+3 */
        fact = fact * (seq_t)(prev + 1);
        fact = fact * (seq_t)(prev + 2);
        fact = fact * (seq_t)(prev + 3);
        fact = fact * (seq_t)(prev + 4);

        product = product * fact;          /* multiply into cumulative product */
    }
    return product;
}
