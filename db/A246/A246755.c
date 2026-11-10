/*
 * A246755  Numbers of the form 2k - 1 such that A246702(k) = 3.
 * https://oeis.org/A246755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th odd d = 2k-1 > 1 with floor((d^2 - 1) /
 *            ord_{d^2}(2)) = 3, i.e. A246702(k) = 3, where ord_{d^2}(2) is
 *            the multiplicative order of 2 mod d^2
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   factor_u64, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/powmod_u64.h"

/* multiplicative order of 2 modulo d^2, d odd > 1 */
static uint64_t A246755_ord2_sq(uint64_t d)
{
    uint64_t m = d * d;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(d, pr, ex, &cnt);
    /* phi(d^2) = d * phi(d) */
    uint64_t phid = d;
    for (size_t i = 0; i < cnt; ++i) {
        phid = phid / pr[i] * (pr[i] - 1);
    }
    uint64_t order = d * phid;
    uint64_t pq[64], eq[64];
    size_t cq = 0;
    factor_u64(order, pq, eq, &cq);
    for (size_t j = 0; j < cq; ++j) {
        uint64_t q = pq[j];
        while ((order % q) == 0) {
            uint64_t cand = order / q;
            if (powmod_u64(2, cand, m) == 1) order = cand;
            else break;
        }
    }
    return order;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t d = 1;
    for (;;) {
        d += 2;
        uint64_t m = d * d;
        uint64_t ord = A246755_ord2_sq(d);
        if ((m - 1) / ord == 3) {
            ++count;
            if (count == n) return (seq_t)d;
        }
    }
}
