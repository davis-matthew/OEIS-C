/*
 * A389832  Multiplicative sequence a(n) with a(p^e) = abs(2 - (e mod 4)) for
 *          prime p and e > 0.
 * https://oeis.org/A389832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} |2 - (e mod 4)| , a(1) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* factor_u64 works only on uint64_t, but idx_t is guaranteed to be
       an unsigned 64-bit integer in all builds. */
    uint64_t nn = (uint64_t)n;

    /* The number of prime factors of a 64‑bit number is at most 64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t ans = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e   = exps[i];
        uint64_t r   = e % 4ULL;                  /* e mod 4 */
        uint64_t val = (r <= 2ULL) ? (2ULL - r) : (r - 2ULL); /* |2 - r| */
        ans *= (seq_t)val;
        if (ans == 0) break;                     /* once zero, stays zero */
    }

    return ans;
}
