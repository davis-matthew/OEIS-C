/*
 * A305804  Möbius-transform of A078898.
 * https://oeis.org/A305804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(n/d)*A078898(d), where A078898(d) = number
 *            of m <= d with the same smallest prime factor as d
 *            (A078898(1)=1)
 * Range:     u8 n <= 513, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9882 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A305804_mu(uint64_t n)
{
    int c = 0;
    if (n == 1) return 1;
    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return 0;
            ++c;
        }
    }
    if (n > 1) ++c;
    return (c & 1) ? -1 : 1;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    uint32_t *spf = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint32_t *cnt = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint32_t *A = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    if (!spf || !cnt || !A) abort();
    for (size_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            for (size_t j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    A[1] = 1;
    for (size_t i = 2; i <= N; ++i) {
        cnt[spf[i]]++;
        A[i] = cnt[spf[i]];
    }
    long long sum = 0;
    for (size_t d = 1; d <= N; ++d) {
        if (N % d == 0) {
            int m = A305804_mu((uint64_t)(N / d));
            if (m) sum += (long long)m * (long long)A[d];
        }
    }
    free(spf);
    free(cnt);
    free(A);
    return (seq_t)sum;
}
