/*
 * A344458  a(n) = Sum_{d|n} d^omega(d).
 * https://oeis.org/A344458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^omega(d) (omega = number of distinct prime
 *            factors)
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Power function that works for both built‑in integer types and arbitrary‑precision
   integer classes (operator* defined in C++ mode). */
static seq_t seq_pow_seq(seq_t base, idx_t exp) {
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res = res * base;
    return res;
}

/* Recursive generator of all divisors of n.
   primes[]  – distinct prime factors of n
   exps[]    – their exponents in n
   pcnt      – number of distinct prime factors
   idx       – current recursive level
   curr_d    – product of chosen prime powers so far
   curr_omega- number of distinct primes selected so far
   res       – running sum of d^omega(d) */
static void gen_divisors(const idx_t *primes, const idx_t *exps,
                         size_t pcnt, size_t idx,
                         seq_t curr_d, idx_t curr_omega,
                         seq_t *res) {
    if (idx == pcnt) {
        seq_t val = seq_pow_seq(curr_d, curr_omega);
        *res += val;
        return;
    }
    idx_t p     = primes[idx];
    idx_t emax  = exps[idx];
    seq_t p_pow = (seq_t)1;
    for (idx_t e = 0; e <= emax; ++e) {
        seq_t new_d = curr_d * p_pow;
        idx_t new_omega = curr_omega + (e > 0 ? 1 : 0);
        gen_divisors(primes, exps, pcnt, idx + 1,
                     new_d, new_omega, res);
        p_pow = p_pow * (seq_t)p;
    }
}

seq_t a(idx_t n) {
    /* Factorise n */
    idx_t temp = n;
    idx_t primes[64];
    idx_t exps[64];
    size_t pcnt = 0;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (temp > 1) {
        primes[pcnt] = temp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    seq_t result = (seq_t)0;
    gen_divisors(primes, exps, pcnt, 0, (seq_t)1, (idx_t)0, &result);
    return result;
}
