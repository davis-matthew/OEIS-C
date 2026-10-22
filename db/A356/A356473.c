/*
 * A356473  Denominator of the average of gcd(i,n) for i = 1..n.
 * https://oeis.org/A356473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn, look
 * Formula:   a(n) = denominator of the average of gcd(i,n) for i = 1..n.
 *            Equivalent closed‑form: a(n) = ∏_{p^e‖n , p∤e} p. This is
 *            derived from the multiplicative formula S(n) = Σ_{d|n} d·φ(n/d)
 *            = ∏_{p^e‖n} ( (e+1)p^e – e·p^{e‑1} )
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation factors n, counts the exponent of each prime
   in S(n), and finally computes the denominator as explained above. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, n ≥ 1 in practice */

    /* Arrays for distinct prime factors and their exponents.
       For n up to 64‑bit there can be at most 64 distinct primes. */
    uint64_t primes[64];
    int      exps[64];
    int      pcnt = 0;

    /* Factor n by trial division. */
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pcnt]   = p;
            exps[pcnt]     = e;
            ++pcnt;
        }
    }
    if (m > 1) {                    /* remaining prime factor */
        primes[pcnt]   = m;
        exps[pcnt]     = 1;
        ++pcnt;
    }

    /* exponents of each prime in S(n) (initialised to 0). */
    int S_exp[64] = {0};

    /* For each prime power p^e, add its contribution to S_exp. */
    for (int i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        int      e = exps[i];

        /* The factor p^{e-1} contributes (e-1) to the exponent of p. */
        if (e > 0)
            S_exp[i] += e - 1;

        /* Remaining factor ((e+1)p - e) may contain other primes of n. */
        if (e > 0) {
            unsigned __int128 term = (unsigned __int128)(e + 1) * p - e;
            for (int j = 0; j < pcnt; ++j) {
                uint64_t q = primes[j];
                /* count how many times q divides term */
                while (term % q == 0) {
                    ++S_exp[j];
                    term /= q;
                }
                if (term == 1) break;   /* early exit if term fully factored */
            }
        }
    }

    /* Compute the denominator: product over primes of p^{e - min(e, S_exp)}. */
    seq_t denom = (seq_t)1;
    for (int i = 0; i < pcnt; ++i) {
        int e          = exps[i];
        int s_e        = S_exp[i];
        int g_exp = (s_e < e) ? s_e : e;   /* exponent in gcd(S(n), n) */

        int d_exp = e - g_exp;             /* exponent in the denominator */
        for (int k = 0; k < d_exp; ++k)
            denom *= (seq_t)primes[i];
    }

    return denom;
}
