/*
 * A342459  a(n) = gcd(A048250(n), A342001(n)).
 * https://oeis.org/A342459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A048250(n), A342001(n)), where A048250(n) =
 *            Product_{p|n} (p+1) and A342001(n) = Sum_{p^v || n} v*rad(n)/p,
 *            with A342001(1) = 0
 * Range:     u8 n <= 1026, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9663 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t A342459_compute(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    int fcnt = 0;

    uint64_t t = (uint64_t)n;
    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            primes[fcnt] = p;
            exps[fcnt] = e;
            ++fcnt;
        }
    }
    if (t > 1) {
        primes[fcnt] = t;
        exps[fcnt] = 1;
        ++fcnt;
    }

    seq_t rad = (seq_t)1;
    seq_t sf = (seq_t)1;
    for (int i = 0; i < fcnt; ++i) {
        rad *= (seq_t)primes[i];
        sf *= ((seq_t)primes[i] + 1);
    }

    seq_t d = (seq_t)0;
    for (int i = 0; i < fcnt; ++i) {
        d += (seq_t)exps[i] * (rad / (seq_t)primes[i]);
    }

    return gcd(sf, d);
}

seq_t a(idx_t n)
{
    return A342459_compute(n);
}
