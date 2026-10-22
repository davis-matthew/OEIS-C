/*
 * A360705  Expansion of Sum_{k>=0} (x * (1 + (-1)^k * x))^k.
 * https://oeis.org/A360705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (-1)^(k*(n-k)) * binomial(n-k,k)
 *            Equivalent: a(2*m) = 1,0,-1 repeat (m mod 3 = 0,1,2) a(2*m+1) =
 *            Fibonacci(2*m+2)
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 46, i64 n <= 92, i128 n <= 184,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* use the Fibonacci routine supplied in the utils library */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* even indices */
    if ((n & 1ull) == 0) {
        idx_t m = n >> 1;               /* m = n/2  */
        /* pattern 1,0,-1 repeating every 3 */
        switch (m % 3ull) {
            case 0: return 1;
            case 1: return 0;
            case 2: return -1;
        }
    }
    /* odd indices: return Fibonacci(n+1)  (since n = 2*m+1) */
    return fib(n + 1);
}
