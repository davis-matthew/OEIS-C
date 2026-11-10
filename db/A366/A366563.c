/*
 * A366563  Number of nonzero terms in row n of A366561(n,k).
 * https://oeis.org/A366563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} abs(sign(A366561(n,k))) = number of distinct
 *            values of gcd(x^2-y^2, n), 1<=x,y<=n (gcd(0,n)=n)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A366563_g(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t N = (uint64_t)n;
    unsigned char *sq = (unsigned char *)calloc((size_t)N, 1);
    unsigned char *seen = (unsigned char *)calloc((size_t)N + 1, 1);
    uint64_t *list = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)N);
    if (!sq || !seen || !list) { free(sq); free(seen); free(list); return (seq_t)0; }
    size_t m = 0;
    for (uint64_t x = 1; x <= N; ++x) {
        uint64_t r = (uint64_t)(((unsigned __int128)x * x) % N);
        if (!sq[r]) { sq[r] = 1; list[m++] = r; }
    }
    uint64_t count = 0;
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < m; ++j) {
            uint64_t d = (list[i] + N - list[j]) % N;
            uint64_t g = d ? A366563_g(d, N) : N;
            if (!seen[g]) { seen[g] = 1; ++count; }
        }
    }
    free(sq); free(seen); free(list);
    return (seq_t)count;
}
