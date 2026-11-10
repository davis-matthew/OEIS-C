/*
 * A370971  A008336(n) is divisible by the product of the primes p such that
 *          n/2 <= p < n; a(n) is the quotient.
 * https://oeis.org/A370971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008336(n)/A055773(n-1), where A008336(1)=1, A008336(k+1)
 *            = A008336(k)/k if k | A008336(k) else A008336(k)*k; tracked as
 *            prime exponents, then divided by the product of primes p with
 *            n/2 <= p < n
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 63, u64 n <= 81, u128 n <= 218,
 *            big any n
 * Validated: standard, pass, 69 terms through n = 2573 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t N = (size_t)n + 2;
    int *ex = (int *)calloc(N, sizeof(int));
    if (!ex) abort();

    for (size_t k = 2; k + 1 <= (size_t)n; ++k) {
        size_t m = k;
        size_t ps[24];
        int fs[24];
        int c = 0;
        for (size_t d = 2; d * d <= m; ++d) {
            if (m % d == 0) {
                int f = 0;
                while (m % d == 0) { m /= d; ++f; }
                ps[c] = d; fs[c] = f; ++c;
            }
        }
        if (m > 1) { ps[c] = m; fs[c] = 1; ++c; }
        int divisible = 1;
        for (int i = 0; i < c; ++i)
            if (ex[ps[i]] < fs[i]) { divisible = 0; break; }
        for (int i = 0; i < c; ++i) {
            if (divisible) ex[ps[i]] -= fs[i];
            else ex[ps[i]] += fs[i];
        }
    }

    /* divide by primes p with 2p >= n, p < n */
    for (size_t p = 2; p < (size_t)n; ++p) {
        int isp = 1;
        for (size_t d = 2; d * d <= p; ++d)
            if (p % d == 0) { isp = 0; break; }
        if (isp && 2 * p >= (size_t)n)
            ex[p] -= 1;
    }

    seq_t r = 1;
    for (size_t p = 2; p < N; ++p) {
        for (int e = 0; e < ex[p]; ++e)
            r *= (seq_t)p;
    }
    free(ex);
    return r;
}
