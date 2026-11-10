/*
 * A382706  a(n) = number of indices k, 1 <= k <= n, such that A064413(k) > k.
 * https://oeis.org/A382706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k : 1 <= k <= n, A064413(k) > k}; computed by
 *            simulating the EKG sequence A064413 (a(1)=1, a(2)=2, then
 *            smallest unused number sharing a factor with the previous term)
 *            and counting
 * Range:     u8 n <= 307, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9676 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static uint32_t *g_cnt = NULL;
static size_t g_m = 0;

static void a064413_build(size_t m)
{
    size_t L = 4 * m + 100;
    uint32_t *spf = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *ptr = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc(L + 1, 1);
    uint32_t *cnt = (uint32_t *)calloc(m + 1, sizeof(uint32_t));
    if (!spf || !ptr || !used || !cnt) exit(1);
    for (size_t i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (size_t j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    uint32_t prev = 2;
    used[1] = 1;
    used[2] = 1;
    cnt[0] = 0;
    if (m >= 1) cnt[1] = 0;
    if (m >= 2) cnt[2] = 0;
    for (size_t k = 3; k <= m; k++) {
        uint32_t best = 0xFFFFFFFFu;
        uint32_t x = prev;
        while (x > 1) {
            uint32_t p = spf[x];
            while (x % p == 0) x /= p;
            if (ptr[p] == 0) ptr[p] = p;
            uint32_t c = ptr[p];
            while (c <= L && used[c]) c += p;
            ptr[p] = c;
            if (c < best) best = c;
        }
        used[best] = 1;
        prev = best;
        cnt[k] = cnt[k - 1] + (best > k ? 1u : 0u);
    }
    free(spf);
    free(ptr);
    free(used);
    free(g_cnt);
    g_cnt = cnt;
    g_m = m;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if ((size_t)n > g_m) {
        size_t m = (size_t)n;
        if (m < 2 * g_m) m = 2 * g_m;
        if (m < 1024) m = 1024;
        a064413_build(m);
    }
    return (seq_t)g_cnt[(size_t)n];
}
