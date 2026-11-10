/*
 * A260672  Table read by rows: T(n,k) = n - A001318(k), k = 0 ..
 *          A193832(n)-1.
 * https://oeis.org/A260672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   T(n,k) = n - A001318(k), k = 0 .. A193832(n+1)-1; a(n) is the
 *            table read by rows (row r has A193832(r+1) terms)
 * Range:     u8 n <= 4458, u16 n <= 34475, u32 n <= 34475, u64 n <= 34475,
 *            u128 n <= 34475, big any n
 * Validated: standard, pass, 95 terms through n = 9676 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A193832_len(idx_t n)
{
    idx_t cum = 0;
    for (idx_t i = 1; ; ++i) {
        idx_t add = (i & 1) ? i : (i >> 1);
        cum += add;
        if (cum >= n) return i;
    }
}

static idx_t A001318_pent(idx_t n)
{
    if (n == 0) return 0;
    idx_t k;
    if (n & 1) {
        k = (n + 1) / 2;
        return k * (3 * k - 1) / 2;
    }
    k = n / 2;
    return k * (3 * k + 1) / 2;
}

seq_t a(idx_t n)
{
    idx_t row = 0;
    idx_t idx = n;
    for (;;) {
        idx_t len = A193832_len(row + 1);
        if (idx < len) break;
        idx -= len;
        row++;
    }
    return (seq_t)(row - A001318_pent(idx));
}
