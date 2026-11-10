/*
 * A337634  Number of numbers k <= n such that both k and k+1 are in A095096.
 * https://oeis.org/A337634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{k : 0 <= k <= n, k and k+1 both have an even number of
 *            1's in their Zeckendorf representation (A095096)}; computed by
 *            direct scan
 * Range:     u8 n <= 1140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9451 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A337634_even(uint64_t m)
{
    uint64_t f[95];
    int c = 2, i;
    bool par = false;
    f[0] = 1; f[1] = 2;
    while (c < 93 && f[c - 1] + f[c - 2] > f[c - 1] && f[c - 1] <= m) {
        f[c] = f[c - 1] + f[c - 2];
        c++;
    }
    i = c - 1;
    while (m > 0) {
        while (f[i] > m) --i;
        par = !par;
        m -= f[i];
    }
    return !par;
}

seq_t a(idx_t n)
{
    uint64_t k;
    uint64_t cnt = 0;
    bool cur = A337634_even(0);
    for (k = 0; k <= (uint64_t)n; k++) {
        bool nxt = A337634_even(k + 1);
        if (cur && nxt) cnt++;
        cur = nxt;
    }
    return (seq_t)cnt;
}
