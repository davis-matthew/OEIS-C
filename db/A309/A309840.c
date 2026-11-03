/*
 * A309840  If n = Sum (2^e_k) then a(n) = Product (Fibonacci(e_k + 3)).
 * https://oeis.org/A309840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{e_k: n = Σ 2^e_k} Fibonacci(e_k + 3), a(0) = 1
 *            Recurrence: a(0)=1; for n>0 let k=floor(log2(n)), m=2^k,
 *            a(n)=Fibonacci(k+3)*a(n-m)
 * Range:     u8 n <= 22, u16 n <= 94, u32 n <= 509, u64 n <= 8188,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 72 terms through n = 9542 (2026-11-01)
 * Helpers:   ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ilog2.h"

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
    seq_t result = 1;
    idx_t tmp = n;
    while (tmp > 0) {
        unsigned k = ilog2(tmp);
        result *= fib((idx_t)k + 3);
        tmp -= ((idx_t)1 << k);
    }
    return result;
}
