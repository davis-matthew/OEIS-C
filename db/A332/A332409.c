/*
 * A332409  a(n) = n!! mod Fibonacci(n).
 * https://oeis.org/A332409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!! mod Fibonacci(n)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Fibonacci number F(n) */
    seq_t fn = fib(n);

    /* If F(n) == 1, the result is always 0. */
    if (fn == (seq_t)1)
        return (seq_t)0;

    seq_t mod = fn;
    seq_t res = 1 % mod;

    for (idx_t k = n; k > 0; ) {
        res = (res * (seq_t)k) % mod;
        if (k <= 2)
            break;        /* avoid unsigned underflow when k == 1 or 2 */
        k -= 2;
    }
    return res;
}
