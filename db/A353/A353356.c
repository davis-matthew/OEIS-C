/*
 * A353356  Numbers k for which A353328(k) > A353329(k). Positions of 1's in
 *          A353354.
 * https://oeis.org/A353356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m with tau(m)*A048675(m) == 2 (mod 3); computed by
 *            sieve search, with A048675(m) mod 3 = Sum e*2^(PrimePi(p)-1) mod
 *            3
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t L = 256;
    for (;;) {
        uint32_t *spf = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
        uint32_t *pidx = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
        uint32_t cnt = 0;
        uint64_t i, j;
        if (!spf || !pidx) { free(spf); free(pidx); return 0; }
        for (i = 2; i <= L; i++) {
            if (spf[i] == 0) {
                cnt++;
                pidx[i] = cnt;
                for (j = i; j <= L; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
        uint64_t found = 0;
        for (uint64_t m = 1; m <= L; m++) {
            uint64_t x = m;
            unsigned s = 0, t = 1;
            while (x > 1) {
                uint32_t p = spf[x];
                unsigned e = 0;
                while (x % p == 0) { x /= p; e++; }
                /* 2^(idx-1) mod 3: 1 if idx odd, 2 if idx even */
                unsigned w = (pidx[p] & 1) ? 1 : 2;
                s = (s + (e % 3) * w) % 3;
                t = (t * ((e + 1) % 3)) % 3;
            }
            if ((s * t) % 3 == 2) {
                found++;
                if (found == (uint64_t)n) {
                    free(spf); free(pidx);
                    return (seq_t)m;
                }
            }
        }
        free(spf); free(pidx);
        L *= 4;
    }
}
