/*
 * A278350  Cototient of Fibonacci(n).
 * https://oeis.org/A278350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fib(n) - φ(Fib(n)) (cototient of Fibonacci(n))
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/totient_seq.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    if (n == 0)      /* offset starts at 1, but guard for safety */
        return 0;

    seq_t fn   = fib(n);           /* F(n)   */
    seq_t phi  = totient_seq(fn);     /* φ(F(n)) */
    return fn - phi;               /* cototient */
}
