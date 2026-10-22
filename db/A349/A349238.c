/*
 * A349238  Reverse the digits in the Zeckendorf representation of n
 *          (A189920).
 * https://oeis.org/A349238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = reverse the bits in the Zeckendorf representation of n
 *            (A189920)
 * Range:     u8 n <= 236, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Build the Fibonacci list 1, 2, 3, 5, … up to ≤ n. */
    seq_t fib[100];
    size_t fib_cnt = 0;
    fib[fib_cnt++] = (seq_t)1;
    if (n >= 2) {
        fib[fib_cnt++] = (seq_t)2;
        while (1) {
            /* Avoid overflow: stop if fib[last] > n - fib[last-1] */
            if (fib[fib_cnt - 2] > (seq_t)n - fib[fib_cnt - 1])
                break;
            fib[fib_cnt++] = fib[fib_cnt - 2] + fib[fib_cnt - 1];
        }
    }

    /* Compute Zeckendorf digits of n. */
    uint8_t bits[100] = {0};
    seq_t rest = (seq_t)n;
    for (int i = (int)fib_cnt - 1; i >= 0; --i) {
        if (rest >= fib[i]) {
            bits[i] = 1;
            rest -= fib[i];
        }
    }

    /* Reverse the digits and evaluate the new Zeckendorf value. */
    seq_t result = 0;
    for (size_t i = 0; i < fib_cnt; ++i) {
        if (bits[fib_cnt - 1 - i])
            result += fib[i];
    }

    return result;
}
