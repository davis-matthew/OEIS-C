/*
 * A377382  a(n) is the smallest number k for which exactly n of its divisors
 *          are interprime numbers (A024675).
 * https://oeis.org/A377382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k with exactly n divisors in A024675 (interprime
 *            numbers); computed by a doubling sieve search
 * Range:     u8 n <= 8, u16 n <= 36, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 43 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t L = 4096;
    for (;;) {
        uint64_t M = 2 * L + 400;
        unsigned char *comp = (unsigned char *)calloc((size_t)M + 1, 1);
        unsigned char *ip = (unsigned char *)calloc((size_t)L + 1, 1);
        uint16_t *cnt = (uint16_t *)calloc((size_t)L + 1, sizeof(uint16_t));
        if (!comp || !ip || !cnt) { free(comp); free(ip); free(cnt); return 0; }
        for (uint64_t i = 2; i * i <= M; i++)
            if (!comp[i])
                for (uint64_t j = i * i; j <= M; j += i) comp[j] = 1;
        uint64_t prev = 0;
        for (uint64_t p = 3; p <= M; p += 2) {
            if (comp[p]) continue;
            if (prev) {
                uint64_t d = (prev + p) / 2;
                if (d <= L) ip[d] = 1;
            }
            prev = p;
        }
        for (uint64_t d = 1; d <= L; d++) {
            if (!ip[d]) continue;
            for (uint64_t m = d; m <= L; m += d) cnt[m]++;
        }
        uint64_t found = 0;
        for (uint64_t k = 1; k <= L; k++) {
            if ((uint64_t)cnt[k] == (uint64_t)n) { found = k; break; }
        }
        free(comp); free(ip); free(cnt);
        if (found) return (seq_t)found;
        L *= 2;
    }
}
