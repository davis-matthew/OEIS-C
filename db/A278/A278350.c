/*
 * A278350  Cototient of Fibonacci(n).
 * https://oeis.org/A278350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fib(n) - φ(Fib(n)) (cototient of Fibonacci(n))
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   calc_phi, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* offset starts at 1, but guard for safety */
        return 0;

    seq_t fn   = fib(n);           /* F(n)   */
    seq_t phi  = calc_phi(fn);     /* φ(F(n)) */
    return fn - phi;               /* cototient */
}
