/*
 * A272530  Position of first occurrence of n in A072347.
 * https://oeis.org/A272530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least k with A072347(k) = n. Search: A072347(k) is the
 *            continuant of the binary digits of k, so write n/m as a
 *            continued fraction for each m coprime to n with 1 <= m < n. Each
 *            partial quotient q becomes the digit block 1(01)^(q-1), and the
 *            last quotient may also be split as (q-1),1. Take t...
 * Range:     u8 n <= 5, u16 n <= 89, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9026 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t A272530_build(const uint64_t *qs, int k, char *buf)
{
    uint64_t len = 0;
    for (int i = 0; i < k; i++) {
        buf[len++] = 1;
        for (uint64_t j = 1; j < qs[i]; j++) {
            buf[len++] = 0;
            buf[len++] = 1;
        }
    }
    return len;
}

static uint64_t A272530_len(const uint64_t *qs, int k)
{
    uint64_t L = 0;
    for (int i = 0; i < k; i++) L += 2 * qs[i] - 1;
    return L;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    uint64_t cap = 2 * (uint64_t)n + 4;
    char *cur = (char *)malloc((size_t)cap);
    char *best = (char *)malloc((size_t)cap);
    if (!cur || !best) { free(cur); free(best); return (seq_t)0; }
    uint64_t bestlen = 0;
    int have = 0;
    uint64_t qs[200];
    for (uint64_t m = 1; m < (uint64_t)n; m++) {
        uint64_t x = (uint64_t)n, y = m;
        int k = 0;
        while (y != 0 && k < 190) {
            uint64_t q = x / y, r = x % y;
            qs[k++] = q;
            x = y; y = r;
        }
        if (x != 1) continue; /* not coprime */
        for (int v = 0; v < 2; v++) {
            int kk = k;
            uint64_t last = qs[k - 1];
            if (v == 1) {
                if (last < 2) continue;
                qs[k - 1] = last - 1;
                qs[k] = 1;
                kk = k + 1;
            }
            uint64_t L = A272530_len(qs, kk);
            if (!have || L <= bestlen) {
                A272530_build(qs, kk, cur);
                int better = 0;
                if (!have || L < bestlen) better = 1;
                else {
                    for (uint64_t i = 0; i < L; i++) {
                        if (cur[i] != best[i]) { better = cur[i] < best[i]; break; }
                    }
                }
                if (better) {
                    memcpy(best, cur, (size_t)L);
                    bestlen = L;
                    have = 1;
                }
            }
            qs[k - 1] = last;
        }
    }
    seq_t res = 0;
    for (uint64_t i = 0; i < bestlen; i++) {
        res = res * 2 + (seq_t)(best[i] ? 1 : 0);
    }
    free(cur);
    free(best);
    return res;
}
