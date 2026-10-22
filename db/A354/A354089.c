/*
 * A354089  Sum of divisors function applied to Pythagorean prime shift: a(n)
 *          = sigma(A348746(n)).
 * https://oeis.org/A354089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma(A348746(n)) where A348746 is multiplicative with
 *            A348744(p): A348744(2)=3, A348744(3)=5, if p≡1 (mod 4) → next
 *            prime ≡1 (mod 4) after p, otherwise A348744(p)=p. Therefore for
 *            n=∏p^e, A348746(n)=∏(A348744(p))^e and a(n) = ∏ σ(A348744(p)^e)
 * Range:     u8 n <= 29, u16 n <= 799, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ---------- simple 64‑bit primality test (deterministic for n < 2^32) ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                 return false;
    if (n % 2 == 0)            return n == 2;
    if (n % 3 == 0)            return n == 3;
    for (uint64_t i = 5; i <= n / i; i += 6) {
        if (n % i == 0)       return false;
        if (n % (i + 2) == 0) return false;
    }
    return true;
}

/* ---------- A348744: shift a prime to the next acceptable prime ---------- */
static uint64_t shift_prime(uint64_t p)
{
    if (p == 2) return 3;
    if (p == 3) return 5;
    if (p % 4 == 1) {
        uint64_t cand = p + 2;
        while (1) {
            if ((cand % 4 == 1) && is_prime_uint64(cand))
                return cand;
            cand += 2;
        }
    }
    return p;  /* p ≡ 3 (mod 4) */
}

/* ---------- sigma of prime power q^e ---------- */
static uint64_t sigma_prime_power(uint64_t q, int e)
{
    uint64_t num = 1;
    for (int i = 0; i < e + 1; ++i)
        num *= q;               /* q^(e+1) */
    return (num - 1) / (q - 1);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, but safe for idx_t == 0 */
    uint64_t m = (uint64_t)n;
    seq_t  result = 1;

    /* factor 2 separately */
    int e = 0;
    while ((m & 1ULL) == 0) {
        ++e;
        m >>= 1;
    }
    if (e > 0) {
        uint64_t q = shift_prime(2);
        uint64_t sigma = sigma_prime_power(q, e);
        result *= (seq_t)sigma;
    }

    /* factor odd primes */
    for (uint64_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) {
            e = 0;
            while (m % d == 0) {
                ++e;
                m /= d;
            }
            uint64_t q = shift_prime(d);
            uint64_t sigma = sigma_prime_power(q, e);
            result *= (seq_t)sigma;
        }
    }

    /* any remaining prime factor > sqrt(original n) */
    if (m > 1) {
        uint64_t q = shift_prime(m);
        uint64_t sigma = sigma_prime_power(q, 1);
        result *= (seq_t)sigma;
    }

    return result;
}
