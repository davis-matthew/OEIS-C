/*
 * A331183  Number of values of k, 1 <= k <= n, with A323898(k) = A323898(n).
 * https://oeis.org/A331183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k : 1<=k<=n, A000120(k)=A000120(n) and
 *            A083254(k)=A083254(n)}, A083254(k)=2*phi(k)-k; computed with a
 *            totient sieve
 * Range:     u8 n <= 15745, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9939 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A331183_pop(uint64_t x)
{
    int c = 0;
    while (x) {
        c += (int)(x & 1);
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    uint64_t N = (uint64_t)n;
    uint64_t *phi = (uint64_t *)malloc((size_t)(N + 1) * sizeof(uint64_t));
    if (!phi)
        return 0;
    for (uint64_t i = 0; i <= N; i++)
        phi[i] = i;
    for (uint64_t p = 2; p <= N; p++) {
        if (phi[p] == p) {
            for (uint64_t m = p; m <= N; m += p)
                phi[m] -= phi[m] / p;
        }
    }
    int pn = A331183_pop(N);
    int64_t vn = 2 * (int64_t)phi[N] - (int64_t)N;
    uint64_t cnt = 0;
    for (uint64_t k = 1; k <= N; k++) {
        if (A331183_pop(k) == pn && 2 * (int64_t)phi[k] - (int64_t)k == vn)
            cnt++;
    }
    free(phi);
    return (seq_t)cnt;
}
