/*
 * A260395  Positions of 0 in the infinite palindromic word at A260394.
 * https://oeis.org/A260395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A260394, found by scanning k =
 *            1, 2, ... and counting k with A260394(k) = 0
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9673 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A260394_f(uint64_t n) {
    for (;;) {
        if (n == 1) return 0;
        if (n == 2 || n == 3) return 1;
        if ((n & (n - 1)) == 0) {
            uint64_t e = 63 - __builtin_clzll((unsigned long long)n);
            n = e - 1;
        } else {
            uint64_t p = (uint64_t)1 << (63 - __builtin_clzll((unsigned long long)n));
            n = 2 * p - n;
        }
    }
}

seq_t a(idx_t n) {
    uint64_t cnt = 0, k = 0;
    while (cnt < (uint64_t)n) {
        k++;
        if (A260394_f(k) == 0) cnt++;
    }
    return (seq_t)k;
}
