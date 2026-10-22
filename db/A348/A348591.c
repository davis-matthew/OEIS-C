/*
 * A348591  a(n) = L(n)*L(n+1) mod F(n+2) where F=A000045 is the Fibonacci
 *          numbers and L = A000032 is the Lucas numbers.
 * https://oeis.org/A348591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 45, u64 n <= 91, u128 n <= 185,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A348591
 * a(n) = L(n)*L(n+1) mod F(n+2)
 * Formula derived in the OEIS entry:
 *   a(n)=3          for odd n ≥ 3
 *   a(n)=F(n+2)-3   for even n ≥ 2
 *   a(0)=0, a(1)=1, a(2)=0
 *
 * The implementation uses the helper fib(idx_t n) from utils/fib.h
 * to obtain Fibonacci numbers.  The Lucas‐number form is not needed
 * because the simplified formula above already gives the desired
 * value for every n.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* handle the first three indices explicitly */
    if (n == 0u) return 0;
    if (n == 1u) return 1;
    if (n == 2u) return 0;

    /* odd indices (n ≥ 3) yield 3 */
    if (n & 1u) return (seq_t)3;

    /* even indices (n ≥ 4) use the simplified formula */
    seq_t f = fib(n + 2u);   /* Fibonacci F(n+2) */
    return f - (seq_t)3;
}
