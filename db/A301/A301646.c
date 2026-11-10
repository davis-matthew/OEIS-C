/*
 * A301646  Numbers k at which A062776(k) reaches a record high.
 * https://oeis.org/A301646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k such that A062776(k) > A062776(j) for all 1 <= j <
 *            k, where A062776(k) = gcd((k+2)!, k^k); computed by a scan,
 *            comparing log(A062776(k))
 * Range:     u8 n <= 25, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static uint64_t *A301646_recs = 0;
static size_t A301646_cnt = 0, A301646_cap = 0;
static uint64_t A301646_next = 1;
static long double A301646_best = -1.0L;

static long double A301646_logg(uint64_t k)
{
    long double s = 0.0L;
    uint64_t m = k;
    uint64_t lim = k + 2;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            uint64_t en = e * k, ef = 0, q = lim;
            while (q) { q /= p; ef += q; }
            s += (long double)(en < ef ? en : ef) * logl((long double)p);
        }
    }
    if (m > 1) {
        uint64_t en = k, ef = 0, q = lim;
        while (q) { q /= m; ef += q; }
        s += (long double)(en < ef ? en : ef) * logl((long double)m);
    }
    return s;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;
    while (A301646_cnt < (size_t)n) {
        uint64_t k = A301646_next++;
        long double g = A301646_logg(k);
        if (g > A301646_best + 1e-12L) {
            A301646_best = g;
            if (A301646_cnt == A301646_cap) {
                A301646_cap = A301646_cap ? A301646_cap * 2 : 256;
                A301646_recs = (uint64_t *)realloc(A301646_recs, A301646_cap * sizeof(uint64_t));
            }
            A301646_recs[A301646_cnt++] = k;
        }
    }
    return (seq_t)A301646_recs[(size_t)n - 1];
}
