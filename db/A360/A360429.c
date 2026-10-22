/*
 * A360429  Inverse Mobius transformation of A034714.
 * https://oeis.org/A360429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} A000005(d)*d^2 Multiplicative: a(p^e) =
 *            sum_{i=0..e} (i+1)*p^(2i)
 * Range:     u8 n <= 7, u16 n <= 71, u32 n <= 7559, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute a(p^e) = Σ_{i=0..e} (i+1)*p^(2i) as a seq_t. */
static seq_t a_prime_power(uint64_t p, uint64_t e)
{
    seq_t sum   = 0;
    seq_t term  = 1;
    seq_t p2    = (seq_t)p * (seq_t)p;

    for (uint64_t i = 0; i <= e; ++i) {
        sum += (seq_t)(i + 1) * term;
        if (i < e) {
            term *= p2;
        }
    }
    return sum;
}

/* Main routine: factor n and multiply contributions from each prime power. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0; protects callers */

    uint64_t temp = (uint64_t)n;   /* local copy for factorisation          */
    seq_t prod = 1;                /* product of prime‑power contributions  */

    /* factor 2 separately */
    uint64_t exp = 0;
    while ((temp & 1ULL) == 0) {
        ++exp;
        temp >>= 1;
    }
    if (exp > 0) {
        prod *= a_prime_power(2, exp);
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                ++exp;
                temp /= p;
            }
            prod *= a_prime_power(p, exp);
        }
    }

    /* any remaining prime factor larger than sqrt(original n) */
    if (temp > 1) {
        prod *= a_prime_power(temp, 1);
    }

    return prod;
}
