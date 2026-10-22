/*
 * A356696  a(n) = Fibonacci(2n-1) - 2^n + binomial(n,2) + 2.
 * https://oeis.org/A356696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2n-1) - 2^n + binomial(n,2) + 2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   binomial, fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/pow2.h"
#include "../utils/binomial.h"

/* Compute A356696 term a(n) for n >= 0 */
seq_t a(idx_t n) {
    /* handle the offset */
    if (n == 0) {
        return (seq_t)2;
    }

    /* Fibonacci index: 2n-1 (n > 0 so result >= 1) */
    idx_t fib_idx = (n << 1) - 1;
    seq_t fib_val = fib(fib_idx);

    /* 2^n */
    seq_t pow2_val = pow2(n);

    /* binomial(n, 2) = n*(n-1)/2   (use the helper for large types) */
    seq_t bin_val = binomial(n, (idx_t)2);

    /* Compute: (fib_val + bin_val + 2) - pow2_val
       The intermediate sum is always >= pow2_val for n >= 1, so no
       unsigned underflow occurs. */
    seq_t sum = fib_val + bin_val + 2;
    return sum - pow2_val;
}
