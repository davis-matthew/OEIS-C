/*
 * A242060  Lpf_3(A242058(n)-3), where lpf_3(n) = lpf(n/3^t) (cf. A020639)
 *          such that 3^t (t>=0) is the maximal power of 3 which divides n.
 * https://oeis.org/A242060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lpf_3(A242058(n) - 3); computed by a direct search over
 *            even m, with trial-division smallest prime factor after
 *            stripping factors of 3
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t A242060_lpf3(uint64_t n)
{
    while (n > 0 && (n % 3ULL) == 0)
        n /= 3ULL;
    if (n <= 1)
        return 1;
    if ((n % 2ULL) == 0)
        return 2;
    for (uint64_t d = 3ULL; d <= n / d; d += 2ULL) {
        if ((n % d) == 0)
            return d;
    }
    return n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    uint64_t m = 6;
    idx_t count = 0;

    while (1) {
        if (A242060_lpf3(m - 1) > A242060_lpf3(m - 3)) {
            ++count;
            if (count == n)
                return (seq_t)A242060_lpf3(m - 3);
        }
        m += 2;
    }
}
