/*
 * A349726  Numbers k >= 1 such that A018804(k) divided by A000203(k) is an
 *          integer.
 * https://oeis.org/A349726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that A018804(k) mod A000203(k) = 0,
 *            where A018804 (Pillai) and sigma are computed multiplicatively:
 *            P(p^e)=p^(e-1)*((e+1)p-e), sigma(p^e)=1+p+...+p^e; found by a
 *            search over k
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A349726_test(uint64_t k)
{
    uint64_t m = k, P = 1, S = 1, p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t e = 0, pw = 1, term = 1, pe1 = 1;
            while (m % p == 0) {
                m /= p;
                if (e > 0) pe1 *= p;
                e++;
                pw *= p;
                term += pw;
            }
            P *= pe1 * ((e + 1) * p - e);
            S *= term;
        }
    }
    if (m > 1) {
        P *= (2 * m - 1);
        S *= (m + 1);
    }
    return (P % S) == 0;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = 0;
    static uint64_t cnt = 0, cap = 0, last = 0;
    while (cnt < (uint64_t)n) {
        uint64_t k = last + 1;
        while (!A349726_test(k)) k++;
        last = k;
        if (cnt == cap) {
            cap = cap ? cap * 2 : 64;
            terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
        }
        terms[cnt++] = k;
    }
    return (seq_t)terms[n - 1];
}
