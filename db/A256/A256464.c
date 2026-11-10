/*
 * A256464  a(n) is the index, in the A256440 triangle, of the first row where
 *          n is the n-th term of the row.
 * https://oeis.org/A256464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m >= n such that n is the n-th term of row m of
 *            A256440 (rows sort 1..m by sigma(k)/k, ties by value); computed
 *            by a sieve of sigma and a scan
 * Range:     u8 n <= 42, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

typedef unsigned __int128 a256464_u128;

static uint64_t *A256464_sieve(uint64_t L)
{
    uint64_t *s = (uint64_t *)calloc((size_t)(L + 1), sizeof(uint64_t));
    if (!s) return NULL;
    for (uint64_t d = 1; d <= L; ++d)
        for (uint64_t j = d; j <= L; j += d)
            s[j] += d;
    return s;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t L = 2 * N + 100;
    for (;;) {
        uint64_t *s = A256464_sieve(L);
        if (!s) return 0;
        uint64_t sn = s[N];
        uint64_t before = 0;
        for (uint64_t k = 1; k < N; ++k)
            if ((a256464_u128)s[k] * N <= (a256464_u128)sn * k)
                ++before;
        uint64_t need = N - 1 - before;
        if (need == 0) {
            free(s);
            return (seq_t)N;
        }
        for (uint64_t k = N + 1; k <= L; ++k) {
            if ((a256464_u128)s[k] * N < (a256464_u128)sn * k) {
                if (--need == 0) {
                    free(s);
                    return (seq_t)k;
                }
            }
        }
        free(s);
        L *= 2;
    }
}
