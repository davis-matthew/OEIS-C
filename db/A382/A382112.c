/*
 * A382112  Distinct elements of A105774.
 * https://oeis.org/A382112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = A382113(n-1) + 1 for n >= 1, where A382113(m)
 *            is the sum of the Fibonacci numbers (1,2,3,5,...) at the
 *            1-positions of the prefix XOR of the Zeckendorf representation
 *            of m
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 64 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A382113_a(idx_t n)
{
    static int init = 0;
    static uint64_t fib[100];
    static int fib_cnt = 0;
    if (!init) {
        fib[0] = 1;
        fib[1] = 2;
        fib_cnt = 2;
        while (fib_cnt < 100) {
            uint64_t x = fib[fib_cnt - 1];
            uint64_t y = fib[fib_cnt - 2];
            if (UINT64_MAX - x < y) break;
            fib[fib_cnt++] = x + y;
        }
        init = 1;
    }

    if (n == 0) return (seq_t)0;

    int lo = 0, hi = fib_cnt - 1, hiIdx = -1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        if (fib[mid] <= (uint64_t)n) {
            hiIdx = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    seq_t result = 0;
    int prefix = 0;
    uint64_t rem = (uint64_t)n;
    for (int j = hiIdx; j >= 0; --j) {
        uint64_t f = fib[j];
        int bit = (rem >= f) ? 1 : 0;
        if (bit) rem -= f;
        prefix ^= bit;
        if (prefix) result += (seq_t)f;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    return A382113_a(n - 1) + 1;
}
