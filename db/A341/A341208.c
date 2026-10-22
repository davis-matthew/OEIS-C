/*
 * A341208  a(n) = F(n+4) * F(n+1) - 4 * (-1)^n where F(n) = A000045(n) are
 *          the Fibonacci numbers.
 * https://oeis.org/A341208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = F(n+4) * F(n+1) - 4 * (-1)^n
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    // Get the required Fibonacci numbers
    seq_t fn1 = fib(n + 1);   // F(n+1)
    seq_t fn4 = fib(n + 4);   // F(n+4)

    // Product of the two Fibonacci numbers
    seq_t prod = fn1 * fn4;

    // Add or subtract 4 depending on the parity of n
    if ((n & 1ULL) == 1ULL) { // n is odd → add 4
        return prod + 4;
    } else {                 // n is even → subtract 4
        return prod - 4;
    }
}
