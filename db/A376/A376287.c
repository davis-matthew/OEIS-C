/*
 * A376287  Index of first occurrence of n in A129363, or 0 if no such number
 *          exists.
 * https://oeis.org/A376287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*k where k is the least index with A129363(k) = n
 *            (A129363(k) = number of ways 2k = p+q, p<=q both twin primes);
 *            computed by sieve and pair counting
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 48 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t L = 1024;
    const uint64_t CAP = (uint64_t)1 << 27;
    while (L <= CAP) {
        unsigned char *pr = (unsigned char *)malloc(L + 3);
        uint32_t *cnt = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
        uint64_t *tw = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
        if (!pr || !cnt || !tw) {
            free(pr); free(cnt); free(tw);
            return (seq_t)0;
        }
        memset(pr, 1, L + 3);
        pr[0] = 0; pr[1] = 0;
        for (uint64_t i = 2; i * i <= L + 2; i++)
            if (pr[i])
                for (uint64_t j = i * i; j <= L + 2; j += i)
                    pr[j] = 0;
        size_t t = 0;
        for (uint64_t p = 3; p <= L; p++) {
            if (pr[p] && (pr[p - 2] || pr[p + 2]))
                tw[t++] = p;
        }
        for (size_t i = 0; i < t; i++) {
            for (size_t j = i; j < t; j++) {
                uint64_t s = tw[i] + tw[j];
                if (s > L) break;
                cnt[s]++;
            }
        }
        uint64_t found = 0;
        for (uint64_t N = 2; N <= L; N += 2) {
            if ((idx_t)cnt[N] == n) { found = N; break; }
        }
        free(pr); free(cnt); free(tw);
        if (found) return (seq_t)found;
        L *= 2;
    }
    return (seq_t)0;
}
