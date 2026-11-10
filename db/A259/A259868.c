/*
 * A259868  a(n) = n*A004141(n).
 * https://oeis.org/A259868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A004141(n), where A004141(n) = max_{k=0..n-2}
 *            |c(n-1,k)|, c(m,k+1) = c(m,k)*2*(m+k)*(m-k)/((2k+1)*(k+1)),
 *            c(m,0) = 1; A004141(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-07)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

static seq_t A004141_a(idx_t n)
{
    uint64_t m = (uint64_t)n - 1;
    seq_t c = 1;
    seq_t best = 1;
    uint64_t k;
    if (m == 0)
        return best;
    for (k = 0; k < m; k++) {
        uint64_t num = 2 * (m + k) * (m - k);
        uint64_t den = (2 * k + 1) * (k + 1);
        uint64_t g = gcd_u64(num, den);
        num /= g;
        den /= g;
        c = c / den;
        c = c * num;
        if (c > best)
            best = c;
    }
    return best;
}

seq_t a(idx_t n)
{
    return (seq_t)n * A004141_a(n);
}
