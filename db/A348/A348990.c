/*
 * A348990  a(n) = n / gcd(n, A003961(n)), where A003961(n) is fully
 *          multiplicative function with a(prime(k)) = prime(k+1).
 * https://oeis.org/A348990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd(n, A003961(n)) where A003961 is the fully
 *            multiplicative function with a(prime)=next prime
 * Range:     u8 n <= 255, u16 n <= 9962, u32 n <= 9962, u64 n <= 9962,
 *            u128 n <= 9962, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* trial division primality test, safe for 64‑bit values */
static bool is_prime_uint64(uint64_t num)
{
    if (num < 2) return false;
    if (num % 2 == 0) return num == 2;
    for (uint64_t i = 3; (__int128)i * i <= num; i += 2)
        if (num % i == 0) return false;
    return true;
}

/* next prime greater than p */
static uint64_t next_prime_uint64(uint64_t p)
{
    uint64_t q = p + 1;
    if (q <= 2) return 2;
    if (q % 2 == 0) q++;
    while (!is_prime_uint64(q))
        q += 2;
    return q;
}

/* factorisation of n into arrays primes[ ], exps[ ].
   *cnt receives the number of distinct primes. */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    *cnt = 0;
    if (n <= 1) return;

    /* factor 2 */
    if ((n & 1) == 0) {
        uint64_t e = 0;
        do { n >>= 1; ++e; } while ((n & 1) == 0);
        primes[(*cnt)++] = 2;
        exps[(*cnt) - 1] = e;
    }

    /* odd divisors up to sqrt(n) */
    for (uint64_t d = 3; (__int128)d * d <= n; d += 2) {
        if (n % d == 0) {
            uint64_t e = 0;
            do { n /= d; ++e; } while (n % d == 0);
            primes[(*cnt)++] = d;
            exps[(*cnt) - 1] = e;
        }
    }
    if (n > 1) {           /* remaining prime factor */
        primes[(*cnt)++] = n;
        exps[(*cnt) - 1] = 1;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0ull) return 0;
    uint64_t un = (uint64_t)n;

    /* factorisation of n */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_uint64(un, primes, exps, &cnt);

    seq_t g = 1;   /* running gcd, built multiplicatively */

    /* iterate over each prime factor p of n */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t q = next_prime_uint64(p);   /* next prime after p */

        /* exponent of q in n, if any */
        uint64_t exp_q = 0;
        for (size_t j = 0; j < cnt; ++j) {
            if (primes[j] == q) {
                exp_q = exps[j];
                break;
            }
        }
        if (exp_q == 0) continue;                /* q not a factor of n */

        uint64_t common = exp_q < exps[i] ? exp_q : exps[i];
        for (uint64_t k = 0; k < common; ++k)
            g *= (seq_t)q;
    }

    return (seq_t)un / g;   /* a(n) = n / gcd(n, A003961(n)) */
}
