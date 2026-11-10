/*
 * A261094  Left inverse of A219640: If n = A219640(k) for some k, then a(n) =
 *          k, otherwise zero.
 * https://oeis.org/A261094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k if n = A219640(k) (distinct values of j -
 *            zeckendorf_popcount(j), ascending), else 0; computed by scanning
 *            j upward
 * Range:     u8 n <= 410, u16 n <= 10946, u32 n <= 10946, u64 n <= 10946,
 *            u128 n <= 10946, big any n
 * Validated: standard, pass, 114 terms through n = 8784 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int zk_popcount(uint64_t k)
{
    static uint64_t fib[95];
    static int cnt = 0;
    if (cnt == 0) {
        fib[0] = 1; fib[1] = 2; cnt = 2;
        while (cnt < 90) { fib[cnt] = fib[cnt-1] + fib[cnt-2]; cnt++; }
    }
    int c = 0;
    int i = cnt - 1;
    while (i >= 0 && fib[i] > k) i--;
    while (k > 0 && i >= 0) {
        if (fib[i] <= k) {
            k -= fib[i];
            c++;
            i -= 2;
        } else {
            i--;
        }
    }
    return c;
}

seq_t a(idx_t n)
{
    uint64_t rank = 0;
    uint64_t prev = 0;
    for (uint64_t k = 0;; k++) {
        uint64_t m = k - (uint64_t)zk_popcount(k);
        if (k == 0 || m > prev) {
            if (m == n) return (seq_t)rank;
            if (m > n) return (seq_t)0;
            rank++;
            prev = m;
        }
    }
}
