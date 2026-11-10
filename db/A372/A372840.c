/*
 * A372840  a(n) = 5-adic valuation of A008336(5*n).
 * https://oeis.org/A372840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A112765(A008336(5*n)); A008336(1) = 1, A008336(k+1) =
 *            A008336(k)/k if k | A008336(k), else A008336(k)*k. Computed by
 *            tracking the prime-exponent vector of A008336(k) up to k = 5n,
 *            then reading off the exponent of 5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9269 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t N = (size_t)n * 5;
    unsigned *spf = (unsigned *)calloc(N + 1, sizeof(unsigned));
    long *e = (long *)calloc(N + 1, sizeof(long));
    if (!spf || !e) abort();
    for (size_t i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (size_t j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (unsigned)i;
        }
    }
    /* e[] holds the exponent vector of A008336(k), starting with k = 1 (value 1) */
    for (size_t k = 1; k < N; k++) {
        bool divides = true;
        size_t m = k;
        while (m > 1) {
            size_t p = spf[m];
            long c = 0;
            while (m % p == 0) { m /= p; c++; }
            if (e[p] < c) { divides = false; break; }
        }
        m = k;
        while (m > 1) {
            size_t p = spf[m];
            long c = 0;
            while (m % p == 0) { m /= p; c++; }
            if (divides) e[p] -= c; else e[p] += c;
        }
    }
    seq_t res = (seq_t)e[5];
    free(spf);
    free(e);
    return res;
}
