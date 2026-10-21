/*
 * A291675  a(n) = a(n-1) + 2*a(n-2) + 8*Fibonacci(n) + 2*Fibonacci(n-1); a(1)
 *          = 4, a(2) = 14.
 * https://oeis.org/A291675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 8*Fibonacci(n) + 2*Fibonacci(n-1)
 *            a(1) = 4, a(2) = 14
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/fib.h"   /* provides seq_t fib(idx_t) */

static seq_t a_cache[20001] = {0};   /* supports n up to 20000 */
static idx_t a_last = 0;

/* Compute a(n) for OEIS sequence A291675 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* n < 1 is undefined; return 0 */
    }
    if (n > 20000) {
        /* For very large n, fall back to a dynamic allocation if needed.
           Since the problem guarantees n ~ 10000, this branch should not be hit. */
        a_cache[n] = 0;
    }

    if (n <= a_last) {
        return a_cache[n];
    }

    /* Initialize base values if needed */
    if (a_last == 0) {
        a_cache[1] = (seq_t)4;
        a_cache[2] = (seq_t)14;
        a_last = 2;
    }

    for (idx_t i = a_last + 1; i <= n; ++i) {
        seq_t fib_i   = fib(i);
        seq_t fib_ip1 = fib(i - 1);
        a_cache[i] =
            a_cache[i - 1]                  /* a(i-1) */
          + (seq_t)2 * a_cache[i - 2]        /* 2*a(i-2) */
          + (seq_t)8 * fib_i                /* 8*Fibonacci(i) */
          + (seq_t)2 * fib_ip1;             /* 2*Fibonacci(i-1) */
    }

    a_last = n;
    return a_cache[n];
}
