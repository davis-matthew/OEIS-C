/*
 * A385653  Least k such that A385652(k) = n.
 * https://oeis.org/A385653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 2 such that A078899(k) = n (found by sieving
 *            gpf and counting; search with doubling limit)
 * Range:     u8 n <= 25, u16 n <= 702, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9102 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t limit = 1024;
    for (;;) {
        uint32_t *gpf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
        uint32_t *cnt = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
        if (!gpf || !cnt) {
            free(gpf);
            free(cnt);
            return (seq_t)0;
        }
        for (uint64_t p = 2; p <= limit; ++p) {
            if (gpf[p] == 0) {
                for (uint64_t m = p; m <= limit; m += p)
                    gpf[m] = (uint32_t)p;
            }
        }
        for (uint64_t k = 2; k <= limit; ++k) {
            uint32_t c = ++cnt[gpf[k]];
            if ((uint64_t)c == (uint64_t)n) {
                free(gpf);
                free(cnt);
                return (seq_t)k;
            }
        }
        free(gpf);
        free(cnt);
        limit *= 4;
    }
}
