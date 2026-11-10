/*
 * A289052  a(1) = 1; a(n) = a(n-A006530(n-1)) + 1 for n > 1.
 * https://oeis.org/A289052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n - A006530(n-1)) + 1, a(1) = 1 (A006530 = greatest
 *            prime factor, with A006530(1)=1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 113 terms through n = 9838 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    size_t N = (size_t)n;
    uint64_t *g = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    uint64_t *v = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    if (!g || !v) {
        free(g);
        free(v);
        return (seq_t)0;
    }
    /* sieve greatest prime factor: later (larger) primes overwrite */
    for (size_t i = 2; i <= N; i++) {
        if (g[i] == 0) {
            for (size_t j = i; j <= N; j += i)
                g[j] = (uint64_t)i;
        }
    }
    g[1] = 1;
    v[1] = 1;
    for (size_t k = 2; k <= N; k++) {
        v[k] = v[k - (size_t)g[k - 1]] + 1;
    }
    uint64_t r = v[N];
    free(g);
    free(v);
    return (seq_t)r;
}
