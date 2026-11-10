/*
 * A349395  Dirichlet convolution of A126760 with Liouville's lambda.
 * https://oeis.org/A349395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A126760(d) * (-1)^bigomega(n/d), i.e. the
 *            Dirichlet convolution of A126760 with Liouville's lambda
 * Range:     u8 n <= 768, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 114 terms through n = 9393 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A126760_a(idx_t n)
{
    seq_t result = 0;
    idx_t m = n;
    while (m > 0) {
        idx_t r = m % 6;
        if (r == 1) { result = ((seq_t)2 * (seq_t)((m - 1) / 6)) + 1; break; }
        if (r == 5) { result = ((seq_t)2 * (seq_t)((m - 5) / 6)) + 2; break; }
        if ((m & 1) == 0) { m >>= 1; continue; }
        if (m % 3 == 0) { m /= 3; continue; }
        break;
    }
    return result;
}

/* parity of number of prime factors with multiplicity */
static int A349395_omega_parity(idx_t m)
{
    int c = 0;
    idx_t p;
    for (p = 2; p * p <= m; p++) {
        while (m % p == 0) { m /= p; c ^= 1; }
    }
    if (m > 1) c ^= 1;
    return c;
}

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    int pn = A349395_omega_parity(n);
    idx_t i;
    for (i = 1; i <= n / i; i++) {
        if (n % i != 0) continue;
        idx_t j = n / i;
        int pi = A349395_omega_parity(i);
        /* term A(i) * lambda(j): parity of Omega(j) = pn ^ pi */
        {
            seq_t v = A126760_a(i);
            if (pn ^ pi) neg += v; else pos += v;
        }
        if (j != i) {
            /* term A(j) * lambda(i) */
            seq_t v = A126760_a(j);
            if (pi) neg += v; else pos += v;
        }
    }
    return pos - neg;
}
