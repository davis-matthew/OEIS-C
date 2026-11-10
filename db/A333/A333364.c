/*
 * A333364  Indices of primes p whose order of primeness A078442(p) is prime.
 * https://oeis.org/A333364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(p) for the n-th prime p whose order of primeness d(p)
 *            is prime, where d(1)=0, d(m)=0 for composite m, and d(prime(k))
 *            = d(k)+1. Computed with a sieve whose limit doubles until n
 *            terms are found
 * Range:     u8 n <= 50, u16 n <= 6421, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 8852 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool A333364_small_prime(int v)
{
    if (v < 2) return false;
    for (int i = 2; i * i <= v; i++)
        if (v % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    size_t L = 1024;
    for (;;) {
        unsigned char *comp = (unsigned char *)calloc(L + 1, 1);
        unsigned char *d = (unsigned char *)calloc(L + 1, 1);
        if (!comp || !d) { free(comp); free(d); return 0; }
        for (size_t i = 2; i * i <= L; i++)
            if (!comp[i])
                for (size_t j = i * i; j <= L; j += i) comp[j] = 1;
        size_t c = 0;
        idx_t found = 0;
        for (size_t i = 2; i <= L; i++) {
            if (comp[i]) continue;
            c++;
            d[i] = (unsigned char)(d[c] + 1);
            if (A333364_small_prime((int)d[i])) {
                found++;
                if (found == n) {
                    free(comp); free(d);
                    return (seq_t)c;
                }
            }
        }
        free(comp); free(d);
        L *= 2;
    }
}
