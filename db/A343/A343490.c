/*
 * A343490  a(n) = Sum_{k=1..n} 4^(gcd(k, n) - 1).
 * https://oeis.org/A343490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * 4^(d-1) = Sum_{k=1..n}
 *            4^(gcd(k,n)-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   phi_pow_p, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/phi_pow_p.h"

/* Recursive enumeration of divisors of n.
   primes, exps hold the factorization of n; num is number of prime factors.
   cur_d: current divisor value (idx_t).
   cur_phi: product of φ(p^{e - a}) contributions for primes already processed. */
static seq_t dfs_div(const idx_t *primes, const idx_t *exps, int num,
                     int idx, idx_t cur_d, seq_t cur_phi)
{
    if (idx == num) {
        idx_t exp = 2 * (cur_d - 1);
        seq_t powv = pow2(exp);
        return cur_phi * powv;
    }

    seq_t sum = 0;
    idx_t p = primes[idx];
    idx_t e = exps[idx];
    idx_t p_pow = 1;
    for (idx_t a = 0; a <= e; ++a) {
        seq_t phi_part = phi_pow_p((seq_t)p, e - a);
        sum += dfs_div(primes, exps, num, idx + 1,
                       cur_d * p_pow, cur_phi * phi_part);
        p_pow *= p;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for 0, but guard */

    /* Factorise n */
    idx_t primes[16];
    idx_t exps[16];
    int num = 0;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[num] = p;
            exps[num] = e;
            ++num;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        primes[num] = temp;
        exps[num] = 1;
        ++num;
    }

    return dfs_div(primes, exps, num, 0, (idx_t)1, (seq_t)1);
}
