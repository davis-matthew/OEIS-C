/*
 * A336455  a(n) = A335915(sigma(n)), where A335915 is a fully multiplicative
 *          sequence with a(2) = 1 and a(p) = A000265(p^2 - 1) for odd primes
 *          p, with A000265(k) giving the odd part of k.
 * https://oeis.org/A336455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A335915(σ(n)) where σ is the sum of divisors. A335915 is
 *            multiplicative with A335915(2)=1 and A335915(p)=odd_part(p^2-1)
 *            for odd primes p. Thus for σ(n)=∏q^e we have a(n)=∏
 *            odd_part(q^2-1)^e (q=2 contributes 1)
 * Range:     u8 n <= 15, u16 n <= 292, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_part, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/odd_part.h"

/* Compute σ(n) = sum of divisors of n.  */
static seq_t sigma_of_n(idx_t n)
{
    seq_t sigma = 1;
    idx_t temp = n;

    /* factor 2 */
    if ((temp & 1) == 0) {
        int e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        /* (2^(e+1)-1) */
        sigma *= seq_pow((seq_t)2, (idx_t)(e + 1)) - 1;
    }

    /* odd factors */
    for (idx_t p = 3; (seq_t)p * (seq_t)p <= (seq_t)temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t numer = seq_pow((seq_t)p, (idx_t)(e + 1)) - 1;
            seq_t denom = (seq_t)(p - 1);
            sigma *= numer / denom;
        }
    }

    /* remaining prime factor >1 */
    if (temp > 1) {
        seq_t numer = seq_pow((seq_t)temp, 2) - 1;
        seq_t denom = (seq_t)(temp - 1);
        sigma *= numer / denom;
    }

    return sigma;
}

/* Compute A335915(σ) given σ.  */
static seq_t a_from_sigma(seq_t sigma)
{
    seq_t result = 1;
    seq_t temp = sigma;

    /* 2 always contributes 1, so skip it */
    while ((temp & 1) == 0)
        temp >>= 1;

    for (idx_t p = 3; (seq_t)p * (seq_t)p <= temp; p += 2) {
        if (temp % (seq_t)p == 0) {
            int e = 0;
            while (temp % (seq_t)p == 0) {
                temp /= p;
                ++e;
            }
            seq_t val = odd_part((seq_t)p * (seq_t)p - 1);
            /* multiply val^e */
            for (int i = 0; i < e; ++i)
                result *= val;
        }
    }

    if (temp > 1) { /* remaining odd prime */
        seq_t val = odd_part((seq_t)temp * (seq_t)temp - 1);
        result *= val;
    }

    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1; undefined for n==0 */
        return 0;
    seq_t sigma = sigma_of_n(n);
    return a_from_sigma(sigma);
}
