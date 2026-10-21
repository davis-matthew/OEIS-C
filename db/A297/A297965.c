/*
 * A297965  a(n) = Fibonacci(binomial(n+3, 3)).
 * https://oeis.org/A297965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(binomial(n+3,3))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t t = n + 3;                     /* n+3 */
    seq_t bin = binomial(t, 3);          /* C(n+3,3) */
    return fib((idx_t)bin);              /* Fibonacci of the binomial */
}
