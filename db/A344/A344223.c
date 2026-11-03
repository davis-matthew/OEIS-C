/*
 * A344223  a(n) = Sum_{k=1..n} tau(gcd(k,n)^n), where tau(n) is the number of
 *          divisors of n.
 * https://oeis.org/A344223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} tau(gcd(k,n)^n)
 * Range:     u8 n <= 11, u16 n <= 41, u32 n <= 329, u64 n <= 6929,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 7415 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursively enumerate all divisors of n and accumulate the sum.
   primes[]   : primes dividing n
   exp[]      : exponents of these primes in n
   pfcount    : number of distinct prime factors
   cur_exps[] : current exponents for the divisor being considered
   sum        : running total of the contribution
*/
static void dfs_divisor(int idx, const idx_t n,
                        const idx_t *primes, const int *exp,
                        int pfcount, int *cur_exps, seq_t *sum)
{
    if (idx == pfcount) {
        /* compute phi(n/d) and tau(d^n) for this divisor d
           d is represented by cur_exps[0..pfcount-1]            */
        seq_t phi_nd = 1;
        seq_t tau_dn = 1;

        for (int i = 0; i < pfcount; ++i) {
            int a = exp[i];
            int b = cur_exps[i];
            seq_t p = primes[i];

            /* phi part: phi(p^{a-b}) */
            int e = a - b;
            if (e > 0) {
                seq_t pow_e = 1;
                for (int j = 0; j < e; ++j)
                    pow_e *= p;
                seq_t pow_e_minus1 = pow_e / p;
                seq_t phi_part = pow_e - pow_e_minus1;
                phi_nd *= phi_part;
            }

            /* tau part: tau(p^{b*n}) = (b*n + 1) if b>0 */
            if (b > 0) {
                tau_dn *= ((seq_t)n * (seq_t)b + 1);
            }
        }

        *sum += phi_nd * tau_dn;
        return;
    }

    /* recurse over current prime exponent */
    for (int ee = 0; ee <= exp[idx]; ++ee) {
        cur_exps[idx] = ee;
        dfs_divisor(idx + 1, n, primes, exp, pfcount, cur_exps, sum);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not needed for OEIS offset 1, but harmless */

    /* factor n */
    idx_t temp = n;
    idx_t primes[16];
    int exp[16];
    int pfcount = 0;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pfcount] = p;
            exp[pfcount] = e;
            ++pfcount;
        }
    }
    if (temp > 1) {
        primes[pfcount] = temp;
        exp[pfcount] = 1;
        ++pfcount;
    }

    int cur_exps[16] = {0};
    seq_t sum = 0;
    dfs_divisor(0, n, primes, exp, pfcount, cur_exps, &sum);
    return sum;
}
