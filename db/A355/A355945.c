/*
 * A355945  a(n) = n minus the smallest positive k such that n divides
 *          k*A276086(k), where A276086 is primorial base exp-function.
 * https://oeis.org/A355945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A355944(n), where A355944(n) is the smallest k>=1
 *            with n | k*A276086(k); computed by a search over k with
 *            A276086(k) taken mod n
 * Range:     u8 n <= 269, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 107 terms through n = 9862 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t a355945_mulmod(uint64_t x, uint64_t y, uint64_t m) {
    return (uint64_t)(((unsigned __int128)x * y) % m);
}

seq_t a(idx_t n)
{
    static const uint64_t pr[15] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
    uint64_t m = (uint64_t)n;
    if (m == 0) return 0;
    for (uint64_t k = 1; k <= m; ++k) {
        uint64_t acc = 1 % m;
        uint64_t x = k;
        for (int i = 0; i < 15 && x > 0; ++i) {
            uint64_t p = pr[i];
            uint64_t d = x % p;
            x /= p;
            for (uint64_t j = 0; j < d; ++j)
                acc = a355945_mulmod(acc, p % m, m);
        }
        if (a355945_mulmod(acc, k % m, m) == 0)
            return (seq_t)(m - k);
    }
    return 0;
}
