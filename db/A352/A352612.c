/*
 * A352612  (n-1)*prod(-p^2 where 2 <= p <= n-2 is prime and relatively prime
 *          to n)*prod(k where both k and (n-k) are composite and relatively
 *          prime to n) (mod n).
 * https://oeis.org/A352612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) * prod( -p^2 mod n : 2 ≤ p ≤ n-2 prime, gcd(p,n)=1
 *            ) * prod( k mod n : 2 ≤ k ≤ n-2, gcd(k,n)=1, k and n-k composite
 *            )
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    return (seq_t)(((__uint128_t)a * b) % mod);
}

static inline bool is_prime_small(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, although n ≥ 1 */
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 2;

    seq_t mod = (seq_t)n;
    seq_t result = mod - 1;        /* (n-1) mod n */

    /* first product over primes p with 2 ≤ p ≤ n-2 and gcd(p,n)=1 */
    for (idx_t p = 2; p + 2 <= n; ++p) {   /* avoid underflow */
        if (!is_prime_small(p)) continue;
        if (gcd((seq_t)p, mod) != 1) continue;

        seq_t pp = (seq_t)(((__uint128_t)p * p) % mod);   /* p^2 mod n */
        seq_t term = (pp == 0) ? 0 : mod - pp;            /* -p^2 mod n */
        result = mul_mod(result, term, mod);
    }

    /* second product over k where k and n-k are composite and gcd(k,n)=1 */
    for (idx_t k = 2; k + 2 <= n; ++k) {
        if (gcd((seq_t)k, mod) != 1) continue;
        if (is_prime_small(k) || is_prime_small(n - k)) continue;  /* need both composite */

        seq_t kk = (seq_t)(k % mod);
        result = mul_mod(result, kk, mod);
    }

    return result % mod;           /* ensure 0 ≤ result < n */
}
