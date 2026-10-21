/*
 * A270312  Numerator of Fibonacci(n)/n.
 * https://oeis.org/A270312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Fib(n)/n = Fib(n)/gcd(Fib(n), n)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   fib, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib.h"
#include "../utils/gcd.h"

/* Computes the nth term of OEIS A270312.
   a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=1, ...  */ 
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* offset starts at 1 */
    seq_t fn = fib(n);                        /* Fib(n) */
    seq_t dn = (seq_t)n;                      /* cast index to seq_t */
    seq_t g = gcd(fn, dn);                    /* g = gcd(Fib(n), n) */
    return fn / g;                            /* numerator after reduction */
}
