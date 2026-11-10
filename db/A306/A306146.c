/*
 * A306146  Numbers k such that A000010(A023900(k)) = A023900(A000010(k)).
 * https://oeis.org/A306146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A000010(A023900(k)) =
 *            A023900(A000010(k)); found by a search over k, with A023900(m) =
 *            (-1)^omega(m) * Prod_{p|m} (p-1)
 * Range:     u8 n <= 20, u16 n <= 1765, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 8707 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* returns prod (p-1) over distinct primes p | m; sets *odd to omega(m) parity */
static idx_t A306146_pm(idx_t m, int *odd)
{
    idx_t r = 1;
    int o = 0;
    idx_t p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            r *= (p - 1);
            o ^= 1;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1) {
        r *= (m - 1);
        o ^= 1;
    }
    *odd = o;
    return r;
}

static idx_t A306146_phi(idx_t n)
{
    idx_t result = 1;
    idx_t m = n;
    idx_t p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            m /= p;
            result *= (p - 1);
            while (m % p == 0) {
                m /= p;
                result *= p;
            }
        }
    }
    if (m > 1)
        result *= (m - 1);
    return result;
}

static int A306146_test(idx_t k)
{
    int o1, o2;
    idx_t P = A306146_pm(k, &o1);
    if (o1)
        return 0;
    idx_t ph = A306146_phi(k);
    idx_t Q = A306146_pm(ph, &o2);
    if (o2)
        return 0;
    return A306146_phi(P) == Q;
}

static idx_t *A306146_terms = 0;
static idx_t A306146_cnt = 0, A306146_cap = 0, A306146_next = 1;

seq_t a(idx_t n)
{
    while (A306146_cnt < n) {
        idx_t k = A306146_next++;
        if (A306146_test(k)) {
            if (A306146_cnt == A306146_cap) {
                A306146_cap = A306146_cap ? A306146_cap * 2 : 1024;
                A306146_terms = (idx_t *)realloc(A306146_terms, A306146_cap * sizeof(idx_t));
            }
            A306146_terms[A306146_cnt++] = k;
        }
    }
    return (seq_t)A306146_terms[n - 1];
}
