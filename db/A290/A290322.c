/*
 * A290322  Sum modulo n of all units u in Z/nZ such that Phi(5,u) is a unit,
 *          where Phi is the cyclotomic polynomial.
 * https://oeis.org/A290322
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look, changed
 * Formula:   a(n) = sum_{u∈U(n), gcd(Phi_5(u),n)=1} u (mod n), where Phi_5(u)
 *            = u^4 + u^3 + u^2 + u + 1 and U(n) are units modulo n
 * Range:     u8 n <= 267, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------   Dependencies   ---------- */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* ----------   Helper: modular multiplication   ---------- */
static inline uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod)
{
    /* use 128‑bit intermediate to avoid overflow (mod < 2^64) */
    __uint128_t prod = ( __uint128_t )a * b;
    return (uint64_t)(prod % mod);
}

/* ----------   Sequence function   ---------- */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* index below offset – not used */

    uint64_t N = n;                       /* work in native 64‑bit */
    uint64_t sum = 0;

    for (uint64_t u = 1; u < N; ++u)
    {
        /* check that u is a unit */
        if (gcd((seq_t)u, (seq_t)n) != (seq_t)1) continue;

        /* compute Phi_5(u) modulo N */
        uint64_t u2 = mulmod(u, u, N);          /* u^2 */
        uint64_t u3 = mulmod(u2, u, N);         /* u^3 */
        uint64_t u4 = mulmod(u3, u, N);         /* u^4 */
        uint64_t phi = (u4 + u3) % N;
        phi = (phi + u2) % N;
        phi = (phi + u)   % N;
        phi = (phi + 1)   % N;

        /* check that Phi_5(u) is also a unit */
        if (gcd((seq_t)phi, (seq_t)n) != (seq_t)1) continue;

        /* add u, keeping sum modulo N */
        sum += u;
        if (sum >= N) sum -= N;
    }

    return (seq_t)sum;      /* sum < N, so it fits into seq_t */
}
