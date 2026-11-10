/*
 * A322805  Lexicographically earliest such sequence a that for all i, j, a(i)
 *          = a(j) => f(i) = f(j), where f(1)=0, f(2)=1, f(n)=-1 for odd
 *          primes, and f(n) = A252463(n) for any other number.
 * https://oeis.org/A322805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence of f (classes numbered
 *            1,2,3,... by first occurrence), where f(1)=0, f(2)=1, f(p)=-1
 *            for odd primes p, and f(n)=A252463(n) otherwise; computed by a
 *            sieve
 * Range:     u8 n <= 472, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9830 (2026-11-08)
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
    uint64_t N = (uint64_t)n;
    uint64_t *spf = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    uint64_t *pp  = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    uint64_t *rk  = (uint64_t *)calloc(N + 4, sizeof(uint64_t));
    uint64_t i, j;
    uint64_t count = 0, answer = 0;
    uint64_t last = 0;

    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = i;
            pp[i] = last ? last : 1;
            last = i;
        }
    }

    for (i = 1; i <= N; i++) {
        uint64_t key;
        if (i == 1) key = 1;
        else if (i == 2) key = 2;
        else if (spf[i] == i) key = 0;       /* odd prime */
        else if ((i & 1u) == 0) key = i / 2 + 2;
        else {
            uint64_t m = i, f = 1;
            while (m > 1) {
                uint64_t p = spf[m], base = pp[p];
                while (m % p == 0) { m /= p; f *= base; }
            }
            key = f + 2;
        }
        if (rk[key] == 0) rk[key] = ++count;
        if (i == N) answer = rk[key];
    }

    free(spf);
    free(pp);
    free(rk);
    return (seq_t)answer;
}
