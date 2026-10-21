/*
 * A268361  Lexicographically least sequence of a certain form that avoids
 *          additive squares.
 * https://oeis.org/A268361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Fibonacci(v2(n) + 2) where v2(n) is the exponent of 2 in
 *            the factorization of n (i.e. the number of trailing zero bits).
 *            This follows from the multiplicative definition: a(2^e) =
 *            Fibonacci(e+2) and a(p^e) = 1 for odd primes. Hence a(n) depends
 *            only on the power of 2 dividing n
 * Range:     u8 n <= 4095, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑compute Fibonacci numbers up to index 65
       (enough for any 64‑bit n, since the maximum exponent of 2
       is 63 and we need Fib(e+2)).  The array is initialized only
       once, regardless of call order. */
    static bool init = false;
    static seq_t fib[66];
    if (!init) {
        fib[0] = 0;
        fib[1] = 1;
        for (int i = 2; i < 66; ++i)
            fib[i] = fib[i - 1] + fib[i - 2];
        init = true;
    }

    /* Count trailing zeros in n to get v2(n).  n is at least 1. */
    idx_t tmp = n;
    int e = 0;
    while ((tmp & 1ULL) == 0) {
        ++e;
        tmp >>= 1;
    }

    /* Return Fibonacci(e+2). */
    return fib[e + 2];
}
