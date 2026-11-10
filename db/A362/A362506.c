/*
 * A362506  a(n) is the least x >= 0 such that A362505(n) = x * y for some y
 *          with the same set of decimal digits as x.
 * https://oeis.org/A362506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least x with A362505(n) = x*y, x and y sharing the same
 *            set of decimal digits; computed by a bounded sieve over pairs
 *            x<=y
 * Range:     u8 n <= 1072, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9841 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static uint32_t *A362506_cache = NULL;
static uint64_t A362506_count = 0;

static void A362506_build(uint64_t B)
{
    uint16_t *mask = (uint16_t *)malloc((size_t)(B + 1) * sizeof(uint16_t));
    uint32_t *best = (uint32_t *)calloc((size_t)(B + 1), sizeof(uint32_t));
    uint64_t i, x, y, p, cnt;
    if (!mask || !best) { free(mask); free(best); return; }
    mask[0] = 1;
    for (i = 1; i <= B; i++) {
        if (i < 10) mask[i] = (uint16_t)(1u << i);
        else mask[i] = (uint16_t)(mask[i / 10] | (1u << (i % 10)));
    }
    for (x = 1; x * x <= B; x++) {
        uint16_t mx = mask[x];
        for (y = x; x * y <= B; y++) {
            if (mask[y] == mx) {
                p = x * y;
                if (best[p] == 0) best[p] = (uint32_t)x;
            }
        }
    }
    cnt = 1;
    for (p = 1; p <= B; p++) if (best[p]) cnt++;
    uint32_t *res = (uint32_t *)malloc((size_t)cnt * sizeof(uint32_t));
    if (res) {
        uint64_t k = 0;
        res[k++] = 0;
        for (p = 1; p <= B; p++) if (best[p]) res[k++] = best[p];
        free(A362506_cache);
        A362506_cache = res;
        A362506_count = cnt;
    }
    free(mask);
    free(best);
}

seq_t a(idx_t n)
{
    uint64_t B = (uint64_t)1 << 20;
    if (n == 0) return 0;
    while (A362506_count < (uint64_t)n) {
        uint64_t before = A362506_count;
        A362506_build(B);
        if (A362506_count == before) return 0; /* allocation failure */
        B *= 4;
    }
    return (seq_t)A362506_cache[(size_t)(n - 1)];
}
