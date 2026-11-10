/*
 * A392842  Number of trailing zeros in a type of Zeckendorf expansion based
 *          on Fibonacci numbers of even index.
 * https://oeis.org/A392842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of trailing zeros in the Zeckendorf representation
 *            (bit 0 = F(2)=1) of F(2k)-n for any even index 2k with F(2k) >
 *            n, i.e. a(n) = A035614(F(2k)-n-1) in the limit; computed here
 *            with 2k = 180
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    typedef unsigned __int128 u128_t;
    static u128_t fib[180];
    static int ready = 0;
    int i;
    if (!ready) {
        fib[0] = 1;
        fib[1] = 2;
        for (i = 2; i < 180; i++) fib[i] = fib[i - 1] + fib[i - 2];
        ready = 1;
    }
    /* fib[i] = F(i+2); fib[178] = F(180) */
    u128_t m = fib[178] - (u128_t)n;
    int minidx = 0;
    int found = 0;
    for (i = 178; i >= 0; i--) {
        if (fib[i] <= m) {
            m -= fib[i];
            minidx = i;
            found = 1;
            if (m == 0) break;
        }
    }
    if (!found) minidx = 0;
    return (seq_t)minidx;
}
