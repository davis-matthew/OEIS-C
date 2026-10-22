/*
 * A374206  The 2-adic valuation of A113177(n), where A113177 is fully
 *          additive with a(p) = Fibonacci(p).
 * https://oeis.org/A374206
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007814(A113177(n))
 * Range:     u8 n <= 9789, u16 n <= 9789, u32 n <= 9789, u64 n <= 9789,
 *            u128 n <= 9789, big any n
 * Helpers:   fib, mul_small, v2_of_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A113177(n) = Σ_{p^e‖n} e · Fib(p)   (full additivity) */

#include <stdint.h>
#include "../utils/fib.h"
#include "../utils/mul_small.h"
#include "../utils/v2_of_seq.h"

/* Compute a(n) for n ≥ 2.  For n < 2 the offset of the sequence is
   not defined; return 0 to satisfy the type signature. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    seq_t sum = 0;      /* cumulative sum of e·Fib(p) */
    idx_t temp = n;

    /* trial division factorisation */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            seq_t fp = fib(p);               /* Fibonacci(p) */
            seq_t term = mul_small(fp, (unsigned int)exp);
            sum += term;
        }
    }

    if (temp > 1)           /* remaining prime factor */
    {
        seq_t fp = fib(temp);
        sum += fp;          /* exponent is 1 */
    }

    return v2_of_seq(sum);
}
