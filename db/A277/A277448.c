/*
 * A277448  a(n) = A277446(n^3).
 * https://oeis.org/A277448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A277446(n^3); A277446 computed by DP over binary blocks
 *            (divisibility chain, max block count)
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Validated: partial, pass, 107 terms through n = 107 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

#define A277448_MAXL 200

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * (seq_t)n * (seq_t)n;
    unsigned char tmp[A277448_MAXL];
    unsigned char bits[A277448_MAXL];
    int len = 0;
    while (m != 0 && len < A277448_MAXL) {
        tmp[len++] = (unsigned char)(m & 1);
        m >>= 1;
    }
    if (len == 0) return (seq_t)0;
    for (int i = 0; i < len; ++i) bits[i] = tmp[len - 1 - i];

    size_t L = (size_t)len;
    seq_t *val = mem_new(seq_t, L * L);
    int *f = (int *)calloc(L * L, sizeof(int));

    for (int s = 0; s < len; ++s) {
        if (!bits[s]) continue;
        seq_t v = 0;
        for (int e = s; e < len; ++e) {
            v = (v << 1) | (seq_t)bits[e];
            val[(size_t)s * L + e] = v;
        }
    }

    for (int s = len - 1; s >= 0; --s) {
        if (!bits[s]) continue;
        for (int e = s; e < len; ++e) {
            if (e == len - 1) {
                f[(size_t)s * L + e] = 1;
                continue;
            }
            if (!bits[e + 1]) continue;
            seq_t d = val[(size_t)s * L + e];
            int best = 0;
            for (int e2 = e + 1; e2 < len; ++e2) {
                int c = f[(size_t)(e + 1) * L + e2];
                if (c <= 0 || c + 1 <= best) continue;
                seq_t w = val[(size_t)(e + 1) * L + e2];
                if ((w % d) == 0) best = c + 1;
            }
            f[(size_t)s * L + e] = best;
        }
    }

    int ans = 0;
    for (int e = 0; e < len; ++e) {
        int c = f[e];
        if (c > ans) ans = c;
    }

    mem_free(val);
    free(f);
    return (seq_t)ans;
}
