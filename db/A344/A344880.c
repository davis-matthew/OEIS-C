/*
 * A344880  a(n) = 1 if for all proper divisors d of n, A011772(d) <
 *          A011772(n), otherwise 0.
 * https://oeis.org/A344880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if S(d) < S(n) for all proper divisors d of n, else 0,
 *            where S = A011772 (smallest m with n | m(m+1)/2); computed by
 *            direct search over divisors
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9597 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A011772_s(uint64_t n)
{
    if (n <= 1) return 1;
    uint64_t limit = 2 * n;
    for (uint64_t m = 1; m < limit; ++m) {
        unsigned __int128 t = ((unsigned __int128)m * (m + 1)) / 2;
        if ((t % n) == 0) return m;
    }
    return limit - 1;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N <= 1) return (seq_t)1;
    uint64_t sn = A011772_s(N);
    for (uint64_t d = 1; d * d <= N; ++d) {
        if (N % d != 0) continue;
        uint64_t e = N / d;
        if (d != N && A011772_s(d) >= sn) return (seq_t)0;
        if (e != d && e != N && A011772_s(e) >= sn) return (seq_t)0;
    }
    return (seq_t)1;
}
