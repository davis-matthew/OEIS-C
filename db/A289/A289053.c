/*
 * A289053  Array T(i,k) read by antidiagonals: position of prime(i)*prime(k)
 *          in A001358.
 * https://oeis.org/A289053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(i,k) = position of prime(i)*prime(i+k-1) in A001358
 *            (semiprimes), read by antidiagonals T(d,1), T(d-1,2), ...,
 *            T(1,d). Position computed as #{semiprimes <= M} = Sum_{p prime,
 *            p^2<=M} (pi(M/p) - pi(p) + 1)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t d = 1;
    while (d * (d + 1) / 2 < N) d++;
    uint64_t j = N - d * (d - 1) / 2;   /* 1..d */
    uint64_t ii = d - j + 1;            /* row */
    uint64_t kk = j;                    /* column */
    uint64_t i1 = ii, i2 = ii + kk - 1; /* prime indices */
    uint64_t need = i2;

    /* find prime(i1) and prime(i2) */
    uint64_t L = 64;
    uint64_t p1 = 0, p2 = 0;
    for (;;) {
        unsigned char *s = (unsigned char *)calloc(L + 1, 1);
        for (uint64_t x = 2; x * x <= L; x++)
            if (!s[x]) for (uint64_t y = x * x; y <= L; y += x) s[y] = 1;
        uint64_t c = 0;
        p1 = 0; p2 = 0;
        for (uint64_t x = 2; x <= L; x++) {
            if (!s[x]) {
                c++;
                if (c == i1) p1 = x;
                if (c == i2) p2 = x;
            }
        }
        free(s);
        if (c >= need) break;
        L *= 2;
    }
    uint64_t M = p1 * p2;

    /* prime counting table up to M/2 */
    uint64_t H = M / 2 + 1;
    unsigned char *c2 = (unsigned char *)calloc(H + 1, 1);
    uint32_t *pi = (uint32_t *)malloc((H + 1) * sizeof(uint32_t));
    for (uint64_t x = 2; x * x <= H; x++)
        if (!c2[x]) for (uint64_t y = x * x; y <= H; y += x) c2[y] = 1;
    pi[0] = 0;
    pi[1] = 0;
    for (uint64_t x = 2; x <= H; x++) pi[x] = pi[x - 1] + (c2[x] ? 0 : 1);

    uint64_t count = 0;
    for (uint64_t p = 2; p * p <= M; p++) {
        if (c2[p]) continue;
        count += (uint64_t)pi[M / p] - pi[p] + 1;
    }
    free(c2);
    free(pi);
    return (seq_t)count;
}
