/*
 * A324501  a(n) = numerator of Sum_{d|n} (1/pod(d)) where pod(k) = the
 *          product of the divisors of k (A007955).
 * https://oeis.org/A324501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{d|n} (1/pod(d)) where pod(d) = product
 *            of all divisors of d
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   gcd_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_seq_t.h"

/* Integer power using exponentiation by squaring */
static seq_t big_pow(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res = res * base;
        base = base * base;
        exp >>= 1;
    }
    return res;
}

/* Euclidean GCD for seq_t */

/* Trial division factorisation: obtains prime factors and exponents of a uint64_t */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[(*cnt)] = p;
            exps[(*cnt)]   = e;
            ++(*cnt);
        }
    }
    if (n > 1) {                    /* remaining prime factor */
        primes[(*cnt)] = n;
        exps[(*cnt)]   = 1;
        ++(*cnt);
    }
}

/* Recursive generation of all divisors of n using pre‑factored primes.
   cur_f[i] stores the exponent currently chosen for prime i. */
static void rec_divs(size_t idx, size_t pn,
                     const uint64_t *pr, const uint64_t *e,
                     uint64_t *cur_f,
                     seq_t cur_d, uint64_t cur_dcnt,
                     seq_t *num, seq_t *den)
{
    if (idx == pn) {
        /* Process this divisor cur_d with divisor count cur_dcnt */
        /* Compute pod(cur_d) = prod_i cur_f[i] * cur_dcnt / 2 */
        seq_t pod_val = 1;
        for (size_t i = 0; i < pn; ++i) {
            if (cur_f[i] == 0) continue;
            uint64_t expi = (cur_f[i] * cur_dcnt) / 2;   /* integer */
            seq_t term = big_pow((seq_t)pr[i], (idx_t)expi);
            pod_val = pod_val * term;
        }
        /* Add 1/pod_val to the current fraction num/den */
        seq_t g = gcd_seq_t(*den, pod_val);
        seq_t pod_div_g = pod_val / g;
        seq_t den_div_g = *den / g;
        seq_t new_num = (*num) * pod_div_g + den_div_g;
        seq_t new_den = (*den) * pod_div_g;
        seq_t g2 = gcd_seq_t(new_num, new_den);
        *num = new_num / g2;
        *den = new_den / g2;
        return;
    }

    /* For each exponent choice from 0 to e[idx] */
    seq_t p_pow = 1;                     /* p^f */
    for (uint64_t f = 0; f <= e[idx]; ++f) {
        cur_f[idx] = f;
        seq_t next_d = cur_d * p_pow;
        uint64_t next_dcnt = cur_dcnt * (f + 1);
        rec_divs(idx + 1, pn, pr, e, cur_f, next_d, next_dcnt, num, den);
        p_pow = p_pow * pr[idx];        /* Prepare p^{f+1} for next iteration */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based */
    uint64_t nn = (uint64_t)n;

    /* Factorise n */
    uint64_t primes[16], exps[16], cur_f[16];
    size_t pn;
    factor_uint64(nn, primes, exps, &pn);

    seq_t num = 0;          /* numerator of the growing sum */
    seq_t den = 1;          /* denominator of the growing sum */

    rec_divs(0, pn, primes, exps, cur_f, 1ULL, 1ULL, &num, &den);

    return num;  /* return only the numerator */
}
