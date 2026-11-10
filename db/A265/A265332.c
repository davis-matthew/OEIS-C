/*
 * A265332  a(n) is the index of the column in A265901 where n appears; also
 *          the index of the row in A265903 where n appears.
 * https://oeis.org/A265332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = A051135(n) = number of times n
 *            appears in A004001 (H(k) = H(H(k-1)) + H(k-H(k-1)), H(1)=H(2)=1)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 136 terms through n = 7926 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *hc = NULL;   /* Hofstadter-Conway sequence, 1-indexed */
static uint64_t *cnt = NULL;  /* cnt[v] = occurrences of v in hc */
static size_t hc_cap = 0;
static size_t hc_len = 0;
static uint64_t hc_max = 0;

static void hc_grow(size_t need)
{
    if (hc_cap >= need) return;
    size_t nc = hc_cap ? hc_cap : 16;
    while (nc < need) nc *= 2;
    hc = (uint64_t *)realloc(hc, nc * sizeof(uint64_t));
    cnt = (uint64_t *)realloc(cnt, nc * sizeof(uint64_t));
    if (!hc || !cnt) abort();
    for (size_t i = hc_cap; i < nc; i++) { hc[i] = 0; cnt[i] = 0; }
    hc_cap = nc;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    if (hc_len == 0) {
        hc_grow(16);
        hc[1] = 1; hc[2] = 1;
        cnt[1] = 2;
        hc_len = 2;
        hc_max = 1;
    }
    while (hc_max <= (uint64_t)n) {
        size_t k = hc_len + 1;
        hc_grow(k + 2);
        uint64_t p = hc[k - 1];
        uint64_t v = hc[p] + hc[k - p];
        hc[k] = v;
        hc_len = k;
        cnt[v]++;
        if (v > hc_max) hc_max = v;
    }
    return (seq_t)cnt[n];
}
