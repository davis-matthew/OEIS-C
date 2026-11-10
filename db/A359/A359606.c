/*
 * A359606  a(n) = 1 if A355690(n) is negative (-1), otherwise 0.
 * https://oeis.org/A359606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if A355690(n) = -1, else 0. A355690 is multiplicative:
 *            a(2^e) has period 6 in e (1,0,-1,-1,0,1), and for odd primes
 *            a(p)=-1 and a(p^k)=0 for k>=2
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9880 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    uint64_t m = (uint64_t)n;
    unsigned e = 0;
    while ((m & 1ULL) == 0) { e++; m >>= 1; }

    int s2;  /* sign of A355690(2^e), 0 if zero */
    switch (e % 6) {
        case 0: case 5: s2 = 1; break;
        case 2: case 3: s2 = -1; break;
        default: s2 = 0; break;
    }
    if (s2 == 0) return (seq_t)0;

    size_t cnt = 0;
    if (m > 1) {
        uint64_t primes[64], exps[64];
        factor_u64(m, primes, exps, &cnt);
        for (size_t i = 0; i < cnt; ++i)
            if (exps[i] > 1) return (seq_t)0;
    }
    if (cnt & 1) s2 = -s2;
    return (seq_t)(s2 < 0 ? 1 : 0);
}
