/*
 * A342662  a(n) = sigma(n) * A064989(n), where A064989 is multiplicative with
 *          a(2^e) = 1 and a(p^e) = prevprime(p)^e for odd primes p, and sigma
 *          is the sum of the divisors of n.
 * https://oeis.org/A342662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = σ(n) * A064989(n) where a(p^e) = q^e * (p^(e+1)-1)/(p-1),
 *            q = 1 for p=2, and q = prev_prime(p) for odd primes
 * Range:     u8 n <= 16, u16 n <= 262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- Helper: integer power for seq_t --------------------------------- */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* --- Main sequence function ------------------------------------------- */
#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1, safeguard */

    seq_t result = 1;
    idx_t m = n;

    /* factor 2 */
    idx_t e2 = 0;
    while ((m & 1) == 0)
    {
        ++e2;
        m >>= 1;
    }
    if (e2 > 0)
    {
        seq_t p2 = 2;
        seq_t p2_e1 = pow_seq(p2, e2 + 1);           /* 2^(e2+1) */
        seq_t sigma2 = (p2_e1 - 1) / (p2 - 1);       /* σ(2^e2) */
        result *= sigma2;                            /* q=1 for p=2 */
    }

    /* odd primes up to sqrt(m) */
    for (idx_t p = 3; p * p <= m; p += 2)
    {
        if (m % p != 0) continue;

        idx_t e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }

        seq_t psq_e1 = pow_seq((seq_t)p, e + 1);
        seq_t sigma = (psq_e1 - 1) / ((seq_t)p - 1);

        seq_t q = (seq_t)prev_prime(p);     /* prevprime(p) for odd p */
        seq_t qpow = pow_seq(q, e);

        result *= sigma;
        result *= qpow;
    }

    /* remaining prime factor (if any) */
    if (m > 1)
    {
        seq_t p = (seq_t)m;
        seq_t psq_e1 = pow_seq(p, 2);        /* e = 1 */
        seq_t sigma = (psq_e1 - 1) / (p - 1);

        seq_t q = (seq_t)prev_prime((idx_t)m);
        seq_t qpow = q;                      /* q^1 */

        result *= sigma;
        result *= qpow;
    }

    return result;
}
