/*
 * A367734  Numbers that have no balanced divisors except for 1.
 * https://oeis.org/A367734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A351112(k) = 1, i.e. no divisor d>1 of k
 *            has phi(d) | sigma(d); found by a sieve (limit doubled until n
 *            terms are found)
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9330 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    uint64_t L = (uint64_t)n * 5 + 200;
    for (;;) {
        uint64_t *phi = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
        uint64_t *sig = (uint64_t *)calloc(L + 1, sizeof(uint64_t));
        unsigned char *bad = (unsigned char *)calloc(L + 1, 1);
        uint64_t i, j;
        for (i = 0; i <= L; i++) phi[i] = i;
        for (i = 2; i <= L; i++) {
            if (phi[i] == i) {
                for (j = i; j <= L; j += i) phi[j] -= phi[j] / i;
            }
        }
        for (i = 1; i <= L; i++)
            for (j = i; j <= L; j += i) sig[j] += i;
        for (i = 2; i <= L; i++) {
            if (!bad[i] && sig[i] % phi[i] == 0) {
                for (j = i; j <= L; j += i) bad[j] = 1;
            }
        }
        uint64_t cnt = 0, res = 0;
        for (i = 1; i <= L; i++) {
            if (!bad[i]) {
                cnt++;
                if (cnt == (uint64_t)n) { res = i; break; }
            }
        }
        free(phi);
        free(sig);
        free(bad);
        if (res) return (seq_t)res;
        L *= 2;
    }
}
