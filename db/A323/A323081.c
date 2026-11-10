/*
 * A323081  Lexicographically earliest such sequence a that for all i, j, a(i)
 *          = a(j) => f(i) = f(j), where f(p) = -(p mod 4) for primes p, and
 *          f(n) = A252463(n) for any other numbers.
 * https://oeis.org/A323081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of first occurrence of f(n) among f(1..n), where
 *            f(p) = -(p mod 4) for primes p and f(n) = A252463(n) otherwise
 *            (restricted growth string; computed by sieve)
 * Range:     u8 n <= 471, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8251 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    uint64_t *spf = (uint64_t *)malloc((N + 2) * sizeof(uint64_t));
    uint64_t *prv = (uint64_t *)malloc((N + 2) * sizeof(uint64_t));
    uint64_t *rk  = (uint64_t *)malloc((N + 8) * sizeof(uint64_t));
    size_t i, j;
    for (i = 0; i <= N + 1; i++) { spf[i] = 0; prv[i] = 0; }
    for (i = 0; i < N + 8; i++) rk[i] = 0;
    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= N; j += i) if (spf[j] == 0) spf[j] = i;
        }
    }
    /* prv[p] = previous prime of prime p */
    {
        uint64_t last = 1;
        for (i = 2; i <= N; i++) {
            if (spf[i] == i) { prv[i] = last; last = i; }
        }
    }
    uint64_t count = 0, result = 0;
    for (size_t m = 1; m <= N; m++) {
        uint64_t key;
        if (m >= 2 && spf[m] == m) {
            key = (uint64_t)(m % 4);   /* 1,2,3 : negative classes */
        } else {
            uint64_t f;
            if (m == 1) f = 1;
            else if ((m & 1u) == 0) f = m >> 1;
            else {
                uint64_t x = m;
                f = 1;
                while (x > 1) {
                    uint64_t p = spf[x];
                    uint64_t b = prv[p];
                    while (x % p == 0) { x /= p; f *= b; }
                }
            }
            key = 4 + f;
        }
        if (rk[key] == 0) rk[key] = ++count;
        if (m == N) result = rk[key];
    }
    free(spf); free(prv); free(rk);
    return (seq_t)result;
}
