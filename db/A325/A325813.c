/*
 * A325813  a(n) = gcd(A034448(n)-n, n-A048146(n)), where A034448 and A048146
 *          are respectively the sum of unitary and non-unitary divisors of n.
 * https://oeis.org/A325813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 4678, u32 n <= 9024, u64 n <= 9024,
 *            u128 n <= 9024, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A325813
   a(n) = gcd(A034448(n)-n, n-A048146(n))
   - A034448(n) : sum of unitary divisors of n
   - A048146(n) : sum of non‑unitary divisors of n
   Equivalently: a(n) = gcd(sigma_star(n)-n, sigma(n)-2*n)
   where sigma(n) is the sum of all divisors and sigma_star(n) is the
   sum of unitary divisors.
   The routine factors n by trial division and evaluates both sums
   multiplicatively.  It works for any n ≥ 1 and is efficient for n ≤ 10⁴.
*/
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for 0, but return 0 for safety */

    /* Factorization */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[64];
    int     exps[64];
    int     cnt = 0;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        primes[cnt] = tmp;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Compute sigma(n) and sigma_star(n) */
    seq_t sigma = 1;
    seq_t sigma_star = 1;
    for (int i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        int   e = exps[i];

        /* p^(e+1) */
        seq_t p_pow = 1;
        for (int j = 0; j < e + 1; ++j)
            p_pow *= p;

        /* sigma factor: (p^(e+1)-1)/(p-1) */
        seq_t num = p_pow - 1;
        seq_t den = p - 1;
        sigma *= num / den;

        /* sigma_star factor: 1 + p^e  (p^e = p_pow / p) */
        seq_t p_pow_e = p_pow / p;
        sigma_star *= (1 + p_pow_e);
    }

    /* Parts of the gcd */
    seq_t part1 = sigma_star - (seq_t)n;          /* sigma_star(n) - n   */
    seq_t diff = sigma;
    seq_t two_n = 2 * (seq_t)n;
    if (diff > two_n)
        diff -= two_n;
    else
        diff = two_n - diff;                      /* |sigma(n)-2n|       */

    return gcd(part1, diff);
}
