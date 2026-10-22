/*
 * A374778  Denominator of the mean abundancy index of the divisors of n.
 * https://oeis.org/A374778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) is the denominator of the mean abundancy index of the
 *            divisors of n. It is computed by factorising n, forming the
 *            product of the sums Σ_{k=0}^{e} σ(p^k)/p^k for each prime power
 *            and then dividing by τ(n)
 * Range:     u8 n <= 12, u16 n <= 2759, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b); */

/* Denominator of the mean abundancy index of the divisors of n. */
seq_t a(idx_t n)
{
    if (n <= 1u)   /* a(1) = 1, and n=0 is unused but harmless */
        return (seq_t)1;

    /* simple trial division factorisation (n <= ~1e4 so this is fast) */
    uint64_t primes[32];
    int exp[32];
    int cnt = 0;
    uint64_t tmp = (uint64_t)n;
    for (uint64_t p = 2u; p * p <= tmp; ++p) {
        if (tmp % p == 0u) {
            int e = 0;
            while (tmp % p == 0u) {
                tmp /= p;
                ++e;
            }
            primes[cnt] = p;
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1u) {            /* remaining prime factor */
        primes[cnt] = tmp;
        exp[cnt] = 1;
        ++cnt;
    }

    /* τ(n) = Π (e_i + 1) */
    seq_t tau = 1;
    for (int i = 0; i < cnt; ++i)
        tau *= (seq_t)(exp[i] + 1);

    /* product over prime powers of Σ σ(p^k)/p^k */
    seq_t num = 1;
    seq_t den = 1;
    for (int i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        int e = exp[i];

        /* Sum S = Σ_{k=0}^e σ(p^k)/p^k as reduced fraction s_num/s_den */
        seq_t s_num = 0;
        seq_t s_den = 1;
        seq_t pk = 1;   /* p^k, starts with k = 0 */

        for (int k = 0; k <= e; ++k) {
            seq_t pk1 = pk * p;                /* p^(k+1)  */
            seq_t term_num = pk1 - 1;          /* (p^(k+1)-1) */
            seq_t term_den = (p - 1) * pk;     /* (p-1)*p^k  */

            /* Add term to current sum */
            seq_t new_num = s_num * term_den + term_num * s_den;
            seq_t new_den = s_den * term_den;
            seq_t g = gcd(new_num, new_den);
            s_num = new_num / g;
            s_den = new_den / g;

            pk = pk1;   /* advance p^k for next iteration */
        }

        /* Multiply this prime's sum into the global product and reduce */
        seq_t prod_num = num * s_num;
        seq_t prod_den = den * s_den;
        seq_t g = gcd(prod_num, prod_den);
        num = prod_num / g;
        den = prod_den / g;
    }

    /* Divide by τ(n) and return the denominator of the reduced mean */
    seq_t final_den = den * tau;
    seq_t g = gcd(num, final_den);
    return final_den / g;
}
