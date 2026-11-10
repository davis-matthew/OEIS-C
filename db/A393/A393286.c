/*
 * A393286  Rectangular array, read by descending antidiagonals; row n gives
 *          the positions in A006881 of numbers whose least prime divisor is
 *          prime(n).
 * https://oeis.org/A393286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(i,j) = number of squarefree semiprimes <= prime(i)*prime(i+j)
 *            (position in A006881), read by descending antidiagonals;
 *            computed via prime counting sieve
 * Range:     u8 n <= 54, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A393286_isp(uint64_t x) {
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A393286_nthprime(uint64_t k) {
    uint64_t c = 0, x = 1;
    while (c < k) {
        x++;
        if (A393286_isp(x)) c++;
    }
    return x;
}

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    uint64_t d = 1;
    while (d * (d + 1) / 2 < nn) d++;
    uint64_t k = nn - d * (d - 1) / 2;
    uint64_t i = k, j = d + 1 - k;
    uint64_t p = A393286_nthprime(i);
    uint64_t q = A393286_nthprime(i + j);
    uint64_t N = p * q;
    uint64_t M = N / 2 + 1;
    unsigned char *comp = (unsigned char *)calloc(M + 1, 1);
    uint32_t *pi = (uint32_t *)malloc((M + 1) * sizeof(uint32_t));
    for (uint64_t x = 2; x * x <= M; x++)
        if (!comp[x])
            for (uint64_t y = x * x; y <= M; y += x) comp[y] = 1;
    pi[0] = 0;
    if (M >= 1) pi[1] = 0;
    for (uint64_t x = 2; x <= M; x++) pi[x] = pi[x - 1] + (comp[x] ? 0 : 1);
    uint64_t total = 0;
    for (uint64_t r = 2; r * r < N; r++) {
        if (comp[r]) continue;
        uint64_t m = N / r;
        if (m > r) total += (uint64_t)pi[m] - (uint64_t)pi[r];
    }
    free(comp);
    free(pi);
    return (seq_t)total;
}
