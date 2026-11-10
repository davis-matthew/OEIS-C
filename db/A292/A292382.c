/*
 * A292382  Base-2 expansion of a(n) encodes the steps where numbers of the
 *          form 4k+2 are encountered when map x -> A252463(x) is iterated
 *          down to 1, starting from x=n.
 * https://oeis.org/A292382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 0; for n > 1, a(n) = 2*a(A252463(n)) + [n == 2 (mod 4)],
 *            where A252463(1)=1, A252463(2n)=n, A252463(2n+1)=prod p^e ->
 *            prev_prime(p)^e
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Validated: standard, pass, 93 terms through n = 1972 (2026-11-08)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static idx_t A292382_prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p) {
        if (is_prime(p)) return p;
    }
}

/* A252463 on native integers; result is always < n so it fits in idx_t */
static idx_t A292382_step(idx_t n)
{
    if (n == 1) return 1;
    if ((n & 1u) == 0u) return n >> 1;

    idx_t m = n;
    idx_t result = 1;
    idx_t p = 3;
    while (p <= m / p) {
        if (m % p == 0) {
            idx_t pp = A292382_prev_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= pp;
            }
        }
        p += 2;
    }
    if (m > 1) {
        result *= A292382_prev_prime(m);
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t w = 1;
    idx_t x = n;
    while (x > 1) {
        if ((x & 3u) == 2u) res += w;
        w *= 2;
        x = A292382_step(x);
    }
    return res;
}
