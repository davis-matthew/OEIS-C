/*
 * A249545  a(n) = number of representations of A020670(n) as x^2 + 7*y^2.
 * https://oeis.org/A249545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (x,y), x,y >= 0, with x^2 + 7*y^2 =
 *            A020670(n), where A020670 lists the values of x^2 + 7*y^2
 *            including 0. Computed with a sieve that counts representations
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9856 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static unsigned short *A249545_res = 0;
static size_t A249545_len = 0;
static unsigned long long A249545_lim = 0;

static void A249545_rebuild(unsigned long long L)
{
    unsigned short *cnt = (unsigned short *)calloc((size_t)(L + 1), sizeof(unsigned short));
    if (!cnt) return;
    for (unsigned long long y = 0; 7ULL * y * y <= L; ++y) {
        unsigned long long b = 7ULL * y * y;
        for (unsigned long long x = 0; b + x * x <= L; ++x)
            cnt[b + x * x]++;
    }
    size_t k = 0;
    for (unsigned long long v = 0; v <= L; ++v)
        if (cnt[v]) ++k;
    unsigned short *r = (unsigned short *)malloc((k + 1) * sizeof(unsigned short));
    if (!r) { free(cnt); return; }
    k = 0;
    for (unsigned long long v = 0; v <= L; ++v)
        if (cnt[v]) r[k++] = cnt[v];
    free(cnt);
    free(A249545_res);
    A249545_res = r;
    A249545_len = k;
    A249545_lim = L;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A249545_lim == 0) A249545_rebuild(1024);
    while (A249545_len < (size_t)n) {
        unsigned long long old = A249545_lim;
        A249545_rebuild(old * 2);
        if (A249545_lim == old) return 0;
    }
    return (seq_t)A249545_res[(size_t)(n - 1)];
}
