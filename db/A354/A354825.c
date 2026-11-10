/*
 * A354825  Dirichlet inverse of A293442, where A293442 is multiplicative with
 *          a(p^e) = A019565(e).
 * https://oeis.org/A354825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a multiplicative; a(p^k) = -Sum_{j=1..k} A019565(j)*a(p^(k-j)),
 *            a(p^0) = 1 (Dirichlet inverse of A293442)
 * Range:     i8 n <= 255, i16 n <= 16384, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 104 terms through n = 9861 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A354825_f(unsigned e)
{
    static const unsigned pr[8] = {2, 3, 5, 7, 11, 13, 17, 19};
    seq_t r = 1;
    unsigned bit = 0;
    while (e) {
        if (e & 1u) r *= (seq_t)pr[bit];
        e >>= 1;
        bit++;
    }
    return r;
}

static seq_t A354825_pp(unsigned k)
{
    seq_t g[70];
    unsigned i, j;
    g[0] = 1;
    for (i = 1; i <= k; i++) {
        seq_t s = 0;
        for (j = 1; j <= i; j++)
            s += A354825_f(j) * g[i - j];
        g[i] = -s;
    }
    return g[k];
}

seq_t a(idx_t n)
{
    seq_t res = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            res *= A354825_pp(e);
        }
    }
    if (m > 1) res *= A354825_pp(1);
    return res;
}
