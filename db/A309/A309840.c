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
 *            u128 n <= 9542, big any n
 * Helpers:   fib, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"
#include "../utils/fib.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t tmp = n;
    while (tmp > 0) {
        unsigned k = floor_log2_uint64(tmp);
        result *= fib((idx_t)k + 3);
        tmp -= ((idx_t)1 << k);
    }
    return result;
}
