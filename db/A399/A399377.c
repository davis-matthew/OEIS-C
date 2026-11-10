/*
 * A399377  a(n) is the least prime p such that p^valuation(A327859(n),p) does
 *          not divide n, or 1 if no such p exists.
 * https://oeis.org/A399377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A020639(A370117(n)); computed directly as the least prime
 *            p with p^e not dividing n, where e is the primorial-base digit
 *            of A003415(n) at p (that digit is the exponent of p in
 *            A327859(n)); 1 if none
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 122 terms through n = 8923 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static uint64_t A399377_deriv(uint64_t n)
{
    if (n < 2) return 0;
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);
    uint64_t r = 0;
    for (size_t i = 0; i < cnt; ++i)
        r += (n / pr[i]) * ex[i];
    return r;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    if (nn == 0) return (seq_t)1;
    uint64_t m = A399377_deriv(nn);
    uint64_t p = 2;
    while (m != 0) {
        uint64_t e = m % p;
        m /= p;
        if (e != 0) {
            uint64_t t = nn;
            for (uint64_t k = 0; k < e; ++k) {
                if (t % p != 0) return (seq_t)p;
                t /= p;
            }
        }
        /* next prime */
        uint64_t q = p + 1;
        for (;; ++q) {
            bool pr = true;
            for (uint64_t d = 2; d * d <= q; ++d)
                if (q % d == 0) { pr = false; break; }
            if (pr) break;
        }
        p = q;
    }
    return (seq_t)1;
}
