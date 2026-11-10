/*
 * A344994  Numbers k such that A173557(k) divides nonzero A051709(k).
 * https://oeis.org/A344994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search/sieve: a(n) = n-th k with A051709(k) > 0 and A173557(k) |
 *            A051709(k), where A051709(k) = sigma(k)+phi(k)-2k and A173557(k)
 *            = Prod_{p|k} (p-1)
 * Range:     u8 n <= 29, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A344994_res = 0;
static uint64_t A344994_cnt = 0;
static uint64_t A344994_lim = 0;

static void A344994_build(uint64_t limit)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    uint64_t *res = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)limit + 1));
    uint64_t cnt = 0, i, j;
    for (i = 2; i <= limit; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (i = 2; i <= limit; i++) {
        uint64_t m = i, sig = 1, ph = 1, g = 1;
        while (m > 1) {
            uint64_t p = spf[m], pp = 1, s = 1;
            while (m % p == 0) {
                m /= p;
                pp *= p;
                s += pp;
            }
            sig *= s;
            ph *= (pp / p) * (p - 1);
            g *= (p - 1);
        }
        {
            uint64_t t = sig + ph;
            if (t > 2 * i) {
                uint64_t v = t - 2 * i;
                if (v % g == 0) res[cnt++] = i;
            }
        }
    }
    free(spf);
    free(A344994_res);
    A344994_res = res;
    A344994_cnt = cnt;
    A344994_lim = limit;
}

seq_t a(idx_t n)
{
    uint64_t idx = (uint64_t)n;
    if (idx == 0) return (seq_t)0;
    if (A344994_lim == 0) A344994_build(16 * idx + 1024);
    while (A344994_cnt < idx) A344994_build(A344994_lim * 2);
    return (seq_t)A344994_res[idx - 1];
}
