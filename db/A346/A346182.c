/*
 * A346182  a(n) = A346463(n) / radical(A346463(n)).
 * https://oeis.org/A346182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A346463(n)/rad(A346463(n)) = Product_{p prime,
 *            e=v_p(A346463(n))>=2} p^(e-1), where A346463(n) =
 *            (4^n-2)*(4^n-1)/Product_{p prime, (p-1)|2n} p =
 *            (2^(2n-1)-1)*(2^(2n)-1)/Product_{odd p, (p-1)|2n} p. The
 *            valuations v_p(2^m-1) come from the lifting-the-exponent lemma:
 *            v_p(2^m-1) = v_p(2^d-1)...
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 85 (2026-11-10)
 * Helpers:   is_prime, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/powmod_u64.h"

/* v_p(2^m - 1) for odd prime p, m >= 1 */
static uint64_t A346182_vp_mersenne(uint64_t p, uint64_t m)
{
    /* multiplicative order of 2 mod p */
    uint64_t d = 1, x = 2 % p;
    while (x != 1)
    {
        x = (x * 2) % p;
        ++d;
    }
    if (m % d != 0)
        return 0;
    /* v_p(2^d - 1) */
    uint64_t v = 0, pk = p;
    while (powmod_u64(2, d, pk) == 1 % pk)
    {
        ++v;
        if (pk > UINT64_MAX / p)
            break;
        pk *= p;
    }
    /* + v_p(m/d) */
    uint64_t q = m / d;
    while (q % p == 0)
    {
        q /= p;
        ++v;
    }
    return v;
}

seq_t a(idx_t n)
{
    seq_t out = (seq_t)1;
    if (n < 1)
        return out;
    uint64_t m1 = 2 * (uint64_t)n - 1;
    uint64_t m2 = 2 * (uint64_t)n;
    uint64_t limit = m2 + 1;
    if (limit < 3511)
        limit = 3511;
    for (uint64_t p = 3; p <= limit; p += 2)
    {
        if (!is_prime((idx_t)p))
            continue;
        uint64_t v = A346182_vp_mersenne(p, m1) + A346182_vp_mersenne(p, m2);
        if (m2 % (p - 1) == 0)
            v -= 1;
        for (uint64_t i = 1; i < v; ++i)
            out *= (seq_t)p;
    }
    return out;
}
