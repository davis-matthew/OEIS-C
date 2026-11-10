/*
 * A264599  Partial sums of A257007.
 * https://oeis.org/A264599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A257007(k), where A257007(k) =
 *            Sum_{b=1..k-1} #{d : d | (k*b-b^2-1), 1 <= d < b}
 * Range:     u8 n <= 17, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A257007_a(idx_t n)
{
    seq_t s = 0;
    if (n <= 2) return s;
    for (idx_t b = 1; b < n; ++b) {
        idx_t m = n * b - b * b - 1;
        if (m == 0) continue;
        uint64_t c = 0;
        for (idx_t d = 1; d < b; ++d)
            if (m % d == 0) ++c;
        s += (seq_t)c;
    }
    return s;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 3; k <= n; ++k)
        total += A257007_a(k);
    return total;
}
