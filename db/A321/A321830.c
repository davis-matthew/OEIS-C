/*
 * A321830  a(n) = Sum_{d|n, n/d==1 mod 4} d^6 - Sum_{d|n, n/d==3 mod 4} d^6.
 * https://oeis.org/A321830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n, n/d==1 (mod 4)} d^6 - Sum_{d|n, n/d==3 (mod 4)}
 *            d^6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1625,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

/* Temporary storage for prime factorisation */
static uint64_t _fprimes[64];
static uint64_t _fexps[64];
static size_t _fcnt;

/* Recursive divisor enumeration and accumulation */
static void _rec(idx_t n, size_t idx, uint64_t cur, seq_t *pos, seq_t *neg) {
    if (idx == _fcnt) {
        uint64_t m = n / cur;          /* m = n/d */
        uint64_t r = m & 3;            /* m % 4 */

        /* d^6 */
        seq_t d6 = (seq_t)cur;
        for (int i = 1; i < 6; ++i) d6 *= (seq_t)cur;

        if (r == 1) *pos += d6;
        else if (r == 3) *neg += d6;
        return;
    }

    uint64_t p = _fprimes[idx];
    unsigned int emax = (unsigned int)_fexps[idx];
    uint64_t d = cur;
    for (unsigned int e = 0; e <= emax; ++e) {
        _rec(n, idx + 1, d, pos, neg);
        d *= p;
    }
}

seq_t a(idx_t n) {
    /* Factorise n */
    _fcnt = 0;
    factor_u64((uint64_t)n, _fprimes, _fexps, &_fcnt);

    seq_t pos = 0, neg = 0;
    _rec(n, 0, 1ULL, &pos, &neg);
    return pos - neg;
}
