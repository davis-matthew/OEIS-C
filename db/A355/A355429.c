/*
 * A355429  Square array T(n, k), n >= 0, k > 0, read by antidiagonals, where
 *          T(0, k) = A001906(k) for k > 0 and where T(n, k) = n - A130312(n)
 *          + A000045(2k + A072649(n)) for n > 0, k > 0.
 * https://oeis.org/A355429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(0,k) = F(2k); T(n,k) = n - F(m) + F(2k+m) for n>0, where m =
 *            A072649(n) = number of distinct Fibonacci numbers <= n; array
 *            read by antidiagonals
 * Range:     u8 n <= 27, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A355429_fib(idx_t n) {
    seq_t x = 0, y = 1;
    for (idx_t i = 0; i < n; i++) {
        seq_t t = x + y;
        x = y;
        y = t;
    }
    return x;
}

static idx_t A355429_cnt(idx_t n) {
    /* number of distinct Fibonacci numbers <= n */
    idx_t c = 0;
    uint64_t x = 1, y = 2;
    while (x <= n) {
        c++;
        uint64_t t = x + y;
        x = y;
        y = t;
    }
    return c;
}

seq_t a(idx_t n) {
    /* largest d with d(d+1)/2 < n */
    uint64_t lo = 0, hi = 4294967295ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid * (mid + 1) / 2 < n) lo = mid; else hi = mid - 1;
    }
    idx_t d = lo;
    idx_t i = n - d * (d + 1) / 2 - 1;
    idx_t row = d - i;
    idx_t k = i + 1;
    if (row == 0) return A355429_fib(2 * k);
    idx_t m = A355429_cnt(row);
    return (seq_t)row - A355429_fib(m) + A355429_fib(2 * k + m);
}
