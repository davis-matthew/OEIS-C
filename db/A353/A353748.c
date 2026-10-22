/*
 * A353748  a(n) = phi(n) - A064989(n), where phi is Euler totient function,
 *          and A064989 shifts the prime factorization one step towards lower
 *          primes.
 * https://oeis.org/A353748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n) – A064989(n) where A064989(n) = Π (prevprime(p)^e)
 *            over the prime factorization n = Π p^e. The function below uses
 *            only integer arithmetic and trial‑division factorization. For
 *            n≤10000 the algorithm runs well under a second
 * Range:     u8 n <= 375, u16 n <= 8752, u32 n <= 8752, u64 n <= 8752,
 *            u128 n <= 8752, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer primality test (trial division) */
static bool is_prime(uint64_t v)
{
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    for (uint64_t d = 3; d * d <= v; d += 2) {
        if (v % d == 0) return false;
    }
    return true;
}

/* previous prime of p (prevprime(2) = 1) */
static uint64_t prevprime_of(uint64_t p)
{
    if (p <= 2) return 1;
    for (uint64_t q = p - 1; q >= 2; --q) {
        if (is_prime(q)) return q;
    }
    return 1; /* unreachable for p>2, but safe */
}

/* main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset 1 – value for n<1 is irrelevant */

    uint64_t temp = (uint64_t)n;
    seq_t phi = (seq_t)n;   /* φ(n) will be updated */
    seq_t prod = 1;         /* product for A064989 */

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* update φ(n) once per distinct prime factor */
            phi = phi / (seq_t)p * (seq_t)(p - 1);

            /* compute prevprime(p)^e */
            uint64_t prevp = prevprime_of(p);
            seq_t term = 1;
            for (uint64_t k = 0; k < e; ++k) term *= (seq_t)prevp;
            prod *= term;
        }
    }

    if (temp > 1) {               /* remaining prime factor */
        uint64_t p = temp;
        phi = phi / (seq_t)p * (seq_t)(p - 1);
        uint64_t prevp = prevprime_of(p);
        prod *= (seq_t)prevp;
    }

    return phi - prod;            /* a(n) = φ(n) – A064989(n) */
}
