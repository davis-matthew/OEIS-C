/*
 * A322873  Ordinal transform of A300721, which is Möbius transform of
 *          A060681.
 * https://oeis.org/A322873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k <= n with c(k) = c(n), where c = A300721,
 *            c(n) = Sum_{d|n} mu(n/d)*A060681(d), A060681(n) = n - n/spf(n),
 *            A060681(1)=0 (sieve-based)
 * Range:     u8 n <= 3237, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 7286 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;
    size_t N = (size_t)n;
    uint32_t *spf = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    signed char *mu = (signed char *)calloc(N + 1, 1);
    long long *av = (long long *)calloc(N + 1, sizeof(long long));
    long long *c = (long long *)calloc(N + 1, sizeof(long long));
    if (!spf || !mu || !av || !c) {
        free(spf); free(mu); free(av); free(c);
        return 0;
    }
    for (size_t i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (size_t j = i; j <= N; j += i)
                if (spf[j] == 0)
                    spf[j] = (uint32_t)i;
        }
    }
    mu[1] = 1;
    av[1] = 0;
    for (size_t m = 2; m <= N; m++) {
        size_t p = spf[m];
        size_t q = m / p;
        mu[m] = (q % p == 0) ? 0 : (signed char)(-mu[q]);
        av[m] = (long long)(m - q);
    }
    for (size_t d = 1; d <= N; d++) {
        if (av[d] == 0)
            continue;
        for (size_t k = 1; k * d <= N; k++) {
            if (mu[k])
                c[k * d] += mu[k] * av[d];
        }
    }
    long long target = c[N];
    unsigned long long cnt = 0;
    for (size_t k = 1; k <= N; k++)
        if (c[k] == target)
            cnt++;
    free(spf); free(mu); free(av); free(c);
    return (seq_t)cnt;
}
