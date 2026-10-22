/*
 * A348492  Greatest common divisor of the arithmetic derivative (A003415) and
 *          Pillai's arithmetical function (A018804).
 * https://oeis.org/A348492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A003415(n), A018804(n)) where A003415(n) is the
 *            arithmetic derivative of n, and A018804(n) is Pillai's
 *            arithmetical function (=∑_{k=1}^n gcd(k,n))
 * Range:     u8 n <= 62, u16 n <= 9866, u32 n <= 9866, u64 n <= 9866,
 *            u128 n <= 9866, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

#ifndef MAX_FACTORS
#define MAX_FACTORS 64
#endif

/* generate all divisors of n and accumulate sum d * φ(n/d) */
static void gen_divs(int idx, int num, idx_t d, idx_t n_div_d,
                     int *exp_d, uint64_t primes[], int exps[],
                     seq_t *sum)
{
    if (idx == num) {
        /* compute φ(n_div_d) */
        seq_t phi = n_div_d;
        for (int i = 0; i < num; ++i) {
            if (exps[i] - exp_d[i] > 0) {
                /* p divides n_div_d */
                phi -= phi / primes[i];
            }
        }
        *sum += (seq_t)d * phi;
        return;
    }

    uint64_t p = primes[idx];
    int emax = exps[idx];
    idx_t d_pow = 1;
    for (int e = 0; e <= emax; ++e) {
        exp_d[idx] = e;
        gen_divs(idx + 1, num, d * d_pow, n_div_d / d_pow,
                 exp_d, primes, exps, sum);
        d_pow *= p;
    }
}

/* main function computing the sequence */
seq_t a(idx_t n)
{
    if (n == 0) { /* undefined but safe guard */
        return 0;
    }

    /* factorisation of n */
    uint64_t primes[MAX_FACTORS];
    int exps[MAX_FACTORS];
    int num_factors = 0;

    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[num_factors] = p;
            exps[num_factors] = e;
            ++num_factors;
        }
    }
    if (temp > 1) {
        primes[num_factors] = temp;
        exps[num_factors]   = 1;
        ++num_factors;
    }

    /* arithmetic derivative */
    seq_t der = 0;
    seq_t sn  = (seq_t)n;
    for (int i = 0; i < num_factors; ++i) {
        der += sn * ((seq_t)exps[i]) / ((seq_t)primes[i]);
    }

    /* Pillai sum */
    seq_t pillai = 0;
    int exp_d[MAX_FACTORS] = {0};
    gen_divs(0, num_factors, 1, n, exp_d, primes, exps, &pillai);

    /* gcd of the two */
    return gcd(der, pillai);
}
