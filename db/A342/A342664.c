/*
 * A342664  Denominator of ratio A342661(n)/A342662(n).
 * https://oeis.org/A342664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A342662(n) / gcd(A342661(n), A342662(n)), where
 *            A342661(n) = n*sigma(A064989(n)), A342662(n) =
 *            sigma(n)*A064989(n)
 * Range:     u8 n <= 18, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-10)
 * Helpers:   factor_u64, gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/is_prime.h"

static uint64_t A342664_prevprime(uint64_t n)
{
    uint64_t p = n - 1;
    while (!is_prime((idx_t)p)) --p;
    return p;
}

static seq_t A342664_sigma(uint64_t k)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    seq_t res = 1;
    if (k <= 1) return 1;
    factor_u64(k, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        seq_t s = 1, t = 1;
        for (uint64_t j = 0; j < ex[i]; ++j) {
            t *= (seq_t)pr[i];
            s += t;
        }
        res *= s;
    }
    return res;
}

seq_t a(idx_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    uint64_t A = 1;
    if (n > 1) {
        factor_u64((uint64_t)n, pr, ex, &cnt);
        for (size_t i = 0; i < cnt; ++i) {
            if (pr[i] == 2) continue;
            uint64_t q = A342664_prevprime(pr[i]);
            for (uint64_t j = 0; j < ex[i]; ++j) A *= q;
        }
    }
    seq_t sa = A342664_sigma(A);
    seq_t sn = A342664_sigma((uint64_t)n);
    seq_t x = (seq_t)n * sa;      /* A342661 */
    seq_t y = sn * (seq_t)A;      /* A342662 */
    seq_t g = gcd(x, y);
    return y / g;
}
