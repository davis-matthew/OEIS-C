/*
 * A253109  a(n) = n ^ (Fibonacci(n) mod n).
 * https://oeis.org/A253109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(Fibonacci(n) mod n)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 12, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

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

seq_t a(idx_t n) {
    seq_t base = (seq_t)n;
    seq_t fibn = fib(n);
    seq_t exp = fibn % base;
    return ipow(base, exp);
}
