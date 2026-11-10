/*
 * A364801  The number of iterations that n requires to reach a fixed point
 *          under the map x -> A022290(x).
 * https://oeis.org/A364801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 0 for n <= 3; a(n) = a(A022290(n)) + 1 for n >= 4
 * Range:     u8 n <= 329, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9938 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A022290_a(idx_t n)
{
    static seq_t fib[66];
    static int ready = 0;
    if (!ready) {
        fib[0] = 0;
        fib[1] = 1;
        for (idx_t i = 2; i < 66; ++i) {
            fib[i] = fib[i-1] + fib[i-2];
        }
        ready = 1;
    }

    seq_t res = 0;
    idx_t bit = 0;
    while (n != 0) {
        if (n & 1) {
            res += fib[bit + 2];
        }
        n >>= 1;
        ++bit;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t x = (seq_t)n;
    while (x >= 4) {
        x = A022290_a((idx_t)x);
        ++count;
    }
    return count;
}
