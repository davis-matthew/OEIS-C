/*
 * A337862  a(n) is the smallest number that can be partitioned into n ways as
 *          the sum of two Moran numbers.
 * https://oeis.org/A337862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least s such that the number of unordered pairs {x,y} of
 *            Moran numbers (A001101) with x+y = s is exactly n; a(0)=0.
 *            Computed by sieve with doubling limit
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 42 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    uint64_t L = 8192;
    for (;;) {
        unsigned char *comp = (unsigned char *)calloc(L + 1, 1);
        uint32_t *cnt = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
        uint32_t *mor = (uint32_t *)malloc((L + 1) * sizeof(uint32_t));
        size_t mc = 0;
        if (!comp || !cnt || !mor) { free(comp); free(cnt); free(mor); return (seq_t)0; }
        comp[0] = 1; if (L >= 1) comp[1] = 1;
        for (uint64_t i = 2; i * i <= L; i++)
            if (!comp[i])
                for (uint64_t j = i * i; j <= L; j += i) comp[j] = 1;
        for (uint64_t k = 1; k <= L; k++) {
            uint64_t ds = 0, t = k;
            while (t) { ds += t % 10; t /= 10; }
            if (k % ds == 0 && !comp[k / ds]) mor[mc++] = (uint32_t)k;
        }
        for (size_t i = 0; i < mc; i++) {
            for (size_t j = i; j < mc; j++) {
                uint64_t s = (uint64_t)mor[i] + mor[j];
                if (s > L) break;
                cnt[s]++;
            }
        }
        uint64_t found = 0;
        for (uint64_t s = 1; s <= L; s++) {
            if (cnt[s] == (uint32_t)n) { found = s; break; }
        }
        free(comp); free(cnt); free(mor);
        if (found) return (seq_t)found;
        L *= 2;
    }
}
