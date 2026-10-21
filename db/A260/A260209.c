/*
 * A260209  Values A such that p=prime(n) satisfies binomial(2p-1, p-1) == 1 +
 *          A*p (mod p^4).
 * https://oeis.org/A260209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( binomial(2p-1, p-1) – 1 ) / p mod p³, where p =
 *            prime(n) (p is the nth prime, n ≥ 1). The procedure computes the
 *            binomial coefficient modulo p⁴ via N = ∏(p + k) and D = ∏k for k
 *            = 1 … p-1, then a(n) = ((N·D⁻¹ mod p⁴) – 1) / p
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 268, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   modinv128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/modinv128.h"

/* ---------- modular arithmetic for 128‑bit values ---------- */
static inline unsigned __int128 modmul128(unsigned __int128 a,
                                          unsigned __int128 b,
                                          unsigned __int128 mod)
{
    unsigned __int128 res = 0;
    a %= mod;
    while (b)
    {
        if (b & 1)
        {
            res += a;
            if (res >= mod) res -= mod;
        }
        a <<= 1;
        if (a >= mod) a -= mod;
        b >>= 1;
    }
    return res;
}

/* ---------- prime table (on‑demand) ---------- */
#define MAX_PRIMES 200000
static uint64_t primes[MAX_PRIMES];
static size_t primes_count = 0;

static inline void ensure_primes(idx_t n)
{
    if (primes_count >= n) return;

    size_t limit;
    if (n < 6)
        limit = 15;
    else
    {
        double dn = (double)n;
        limit = (size_t)(dn * (log(dn) + log(log(dn))) + 10.0);
    }

    unsigned char *is_comp = (unsigned char *)calloc(limit + 1, 1);
    if (!is_comp) return;          /* allocation failure – caller will fail */
    size_t count = 0;

    for (size_t i = 2; i <= limit && count < n; ++i)
    {
        if (!is_comp[i])
        {
            if (count < n) primes[count++] = (uint64_t)i;
            if (i * i <= limit)
                for (size_t j = i * i; j <= limit; j += i) is_comp[j] = 1;
        }
    }

    free(is_comp);
    primes_count = count;

    if (primes_count < n)
        ensure_primes(n + 1000);   /* increase limit and retry */
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for index 0 */

    ensure_primes(n);
    uint64_t p = primes[n - 1];

    unsigned __int128 M = (unsigned __int128)p * p * p * p;  /* p⁴  */

    unsigned __int128 N = 1;   /* numerator product */
    unsigned __int128 D = 1;   /* denominator product */

    for (uint64_t k = 1; k <= p - 1; ++k)
    {
        N = modmul128(N, (unsigned __int128)(p + k), M);
        D = modmul128(D, (unsigned __int128)k, M);
    }

    unsigned __int128 invD = modinv128(D, M);
    unsigned __int128 binom = modmul128(N, invD, M);

    unsigned __int128 diff = binom - 1;
    unsigned __int128 A = diff / (unsigned __int128)p;  /* integer division */

    return (seq_t)A;   /* cast to the build's seq_t type */
}
