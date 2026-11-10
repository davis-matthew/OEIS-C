/*
 * A330704  a(n) is the least number k for which A330437(k) = n.
 * https://oeis.org/A330704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least k with A330437(k) = n, where A330437(k) is the
 *            length of the trajectory of k under m -> m-1+m/gpf(m) (prime or
 *            1 reached: length 1). Computed by a sieve of greatest prime
 *            factors with doubling limit
 * Range:     u8 n <= 7, u16 n <= 36, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-08)
 * Helpers:   is_prime, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    uint64_t L = 1024;
    for (;;) {
        uint64_t M = 2 * L + 16;
        uint32_t *gpf = (uint32_t *)calloc((size_t)M + 1, sizeof(uint32_t));
        unsigned char *len = (unsigned char *)calloc((size_t)M + 1, 1);
        if (!gpf || !len) { free(gpf); free(len); return 0; }
        for (uint64_t p = 2; p <= M; p++) {
            if (gpf[p] == 0) {
                for (uint64_t j = p; j <= M; j += p) gpf[j] = (uint32_t)p;
            }
        }
        for (uint64_t k = M; k >= 1; k--) {
            if (k == 1 || gpf[k] == (uint32_t)k) {
                len[k] = 1;
            } else {
                uint64_t x = k - 1 + k / gpf[k];
                uint64_t steps = 1;
                while (x > M) {
                    if (is_prime((idx_t)x)) { steps += 1; x = 0; break; }
                    uint64_t g = largest_prime_factor(x);
                    x = x - 1 + x / g;
                    steps++;
                }
                uint64_t total = steps + (x ? len[x] : 0);
                len[k] = (unsigned char)(total > 255 ? 255 : total);
            }
        }
        for (uint64_t k = 1; k <= L; k++) {
            if ((idx_t)len[k] == n && len[k] != 255) {
                free(gpf); free(len);
                return (seq_t)k;
            }
        }
        free(gpf); free(len);
        L *= 2;
    }
}
