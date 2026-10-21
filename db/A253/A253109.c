/*
 * A253109  a(n) = n ^ (Fibonacci(n) mod n).
 * https://oeis.org/A253109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(Fibonacci(n) mod n)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 12, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t base = (seq_t)n;
    seq_t fibn = fib(n);
    seq_t exp = fibn % base;
    return big_pow(base, exp);
}
