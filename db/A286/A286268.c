/*
 * A286268  Totient superabundant numbers: numbers n such that A092693(n)/n >
 *          A092693(m)/m for all m < n.
 * https://oeis.org/A286268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th record of A092693(m)/m (strict), found by sieving
 *            phi and iterated phi sums, with a(1) = 1
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define TSA_CAP ((uint32_t)1 << 24)

static uint64_t tsa_rec[64];
static int tsa_cnt = 0;
static uint32_t tsa_done = 0;

static void tsa_search(uint32_t lim)
{
    uint32_t *phi = (uint32_t *)malloc(((size_t)lim + 1) * sizeof(uint32_t));
    uint32_t *s = (uint32_t *)malloc(((size_t)lim + 1) * sizeof(uint32_t));
    uint32_t i, p, j;
    uint64_t bs = 0, bm = 1;
    if (!phi || !s) abort();
    for (i = 0; i <= lim; i++) phi[i] = i;
    for (p = 2; p <= lim; p++) {
        if (phi[p] == p) {
            for (j = p; j <= lim; j += p) {
                phi[j] = phi[j] / p * (p - 1);
                if (j > lim - p) break;
            }
        }
    }
    s[0] = 0;
    if (lim >= 1) s[1] = 0;
    tsa_cnt = 0;
    tsa_rec[tsa_cnt++] = 1;
    for (i = 2; i <= lim; i++) {
        s[i] = phi[i] + s[phi[i]];
        if ((uint64_t)s[i] * bm > bs * (uint64_t)i) {
            bs = s[i];
            bm = i;
            if (tsa_cnt < 64) tsa_rec[tsa_cnt++] = i;
        }
    }
    free(phi);
    free(s);
    tsa_done = lim;
}

seq_t a(idx_t n)
{
    if (n < 1 || n > 64) return 0;
    while ((idx_t)tsa_cnt < n && tsa_done < TSA_CAP) {
        uint32_t next = tsa_done ? tsa_done * 4 : ((uint32_t)1 << 17);
        if (next > TSA_CAP) next = TSA_CAP;
        tsa_search(next);
    }
    if ((idx_t)tsa_cnt < n) return 0;
    return (seq_t)tsa_rec[n - 1];
}
