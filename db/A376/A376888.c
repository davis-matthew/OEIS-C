/*
 * A376888  The sum of divisors of n that are products of factors of the form
 *          p^(k!) with multiplicities not larger than their multiplicity in
 *          n, where p is a prime and k >= 1, when the factorization of n is
 *          uniquely done using the factorial-base representation of the
 *          exponents in the prime factorization of n.
 * https://oeis.org/A376888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, base
 * Formula:   a(n) = Sum of divisors of n that can be expressed as product of
 *            prime powers p^(k!·m_k) where m_k ≤ the k‑th factorial‑base
 *            digit of the exponent of p in n. Equivalent multiplicative
 *            definition: a(p^e) = ∏_{k≥1} (p^(k!·(d_k+1)) – 1)/(p^(k!) – 1),
 *            with e = Σ d_k·k! (factorial base representation)
 * Range:     u8 n <= 16, u16 n <= 1040, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Convert an unsigned integer to seq_t; the type seq_t supports standard
   arithmetic operators, including multiplication and division. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Compute a(p^e) for a prime power. */
static seq_t a_prime_pow(uint64_t p, uint64_t e)
{
    seq_t prime = (seq_t)p;
    seq_t subprod = 1;

    idx_t k = 1;          /* current position in factorial base   */
    idx_t fact = 1;       /* k!                                 */
    uint64_t rem = e;

    while (rem > 0)
    {
        /* factorial base digit: base = k+1 */
        uint64_t d = rem % (k + 1);
        rem /= (k + 1);

        if (d > 0)
        {
            seq_t denom   = pow_seq(prime, fact);
            seq_t numer   = pow_seq(prime, fact * (d + 1));
            seq_t term    = (numer - 1) / (denom - 1);
            subprod      *= term;
        }

        /* advance to next factorial place */
        k++;
        fact *= k;        /* now fact = k! */
    }

    return subprod;
}

/* Main sequence function.  We factor n with a simple trial division
   (sufficient for n up to at least 10 000 and far beyond, given the
   length of the divisor chain).  The result is multiplicative. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined below offset, but safe */

    uint64_t temp = (uint64_t)n;
    seq_t total = 1;

    /* factor out all 2's first */
    if (temp % 2 == 0)
    {
        uint64_t exp = 0;
        while (temp % 2 == 0) { temp /= 2; exp++; }
        total *= a_prime_pow(2, exp);
    }

    /* trial division by odd numbers */
    for (uint64_t p = 3; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            uint64_t exp = 0;
            while (temp % p == 0) { temp /= p; exp++; }
            total *= a_prime_pow(p, exp);
        }
    }

    /* any remaining prime factor > sqrt(original) */
    if (temp > 1)
        total *= a_prime_pow(temp, 1);

    return total;
}
