/*
 * A322872  Ordinal transform of A171462, where A171462(n) = n - A052126(n).
 * https://oeis.org/A322872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k <= n with b(k) = b(n), where b(k) = k -
 *            A052126(k), b(1) = 0 (ordinal transform of A171462)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9475 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    uint32_t *lp = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint32_t *cnt = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    if (!lp || !cnt) {
        free(lp);
        free(cnt);
        return (seq_t)0;
    }
    for (size_t p = 2; p <= N; p++) {
        if (lp[p] == 0) {
            for (size_t m = p; m <= N; m += p)
                lp[m] = (uint32_t)p;
        }
    }
    uint32_t res = 0;
    for (size_t k = 1; k <= N; k++) {
        size_t b = (k == 1) ? 0 : k - k / lp[k];
        res = ++cnt[b];
    }
    free(lp);
    free(cnt);
    return (seq_t)res;
}
