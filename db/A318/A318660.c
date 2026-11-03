/*
 * A318660  Remainder when A064988(n) is divided by n.
 * https://oeis.org/A318660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{p^e‖n} prime(p)^e) mod n where prime(p) denotes the
 *            p‑th prime number (1‑indexed)
 * Range:     u8 n <= 284, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 110 terms through n = 9596 (2026-11-03)
 * Helpers:   prime_nth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A318660: A064988(n) mod n, A064988 being multiplicative with
 * a(p^e) = prime(p)^e. n is factored by trial division and the product taken
 * modulo n in 128-bit steps; prime(p) comes from a prime table that grows as
 * needed (the original's table stopped at 200000, below prime(17985)). */

#include <stdint.h>
#include "../utils/prime_nth.h"

static uint64_t mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((unsigned __int128)a * b % m);
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    const uint64_t N = (uint64_t)n;
    uint64_t t = N, r = 1 % N;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p) continue;
        uint64_t q = prime_nth(p) % N;
        while (t % p == 0) {
            t /= p;
            r = mulmod(r, q, N);
        }
    }
    if (t > 1) r = mulmod(r, prime_nth(t) % N, N);
    return (seq_t)r;
}
