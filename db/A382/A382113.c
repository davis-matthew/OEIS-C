/*
 * A382113  Gray code transformation of the Zeckendorf representation of n.
 * https://oeis.org/A382113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A022290(A006068(A003714(n)))
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   = sum of Fibonacci numbers whose positions in the prefix‑XOR
 *     of the Zeckendorf representation of n are 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute Fibonacci numbers 1,2,3,5,... up to 2^64-1. */
    static int init = 0;
    static idx_t fib[100];
    static int fib_cnt = 0;
    if (!init) {
        fib[0] = 1;
        fib[1] = 2;
        fib_cnt = 2;
        while (1) {
            idx_t a = fib[fib_cnt - 1];
            idx_t b = fib[fib_cnt - 2];
            if (UINT64_MAX - a < b)   /* would overflow */
                break;
            fib[fib_cnt++] = a + b;
            if (fib_cnt >= 100)       /* safety guard */
                break;
        }
        init = 1;
    }

    if (n == 0U)
        return 0;

    /* Find highest Fibonacci <= n. */
    int lo = 0, hi = fib_cnt - 1, hiIdx = -1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        if (fib[mid] <= n) {
            hiIdx = mid;
            lo = mid + 1;
        } else
            hi = mid - 1;
    }

    seq_t result = 0;
    uint8_t prefix = 0;
    for (int j = hiIdx; j >= 0; --j) {
        idx_t f = fib[j];
        uint8_t bit = (n >= f) ? 1 : 0;
        if (bit)
            n -= f;
        prefix ^= bit;
        if (prefix)
            result += (seq_t)f;
    }
    return result;
}
