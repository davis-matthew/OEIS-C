/*
 * A306650  a(n) = denominator of Sum_{d|n} (d/sigma(d)) where sigma(k) = the
 *          sum of the divisors of k (A000203).
 * https://oeis.org/A306650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of \sum_{d|n} d / sigma(d), where sigma(k) is
 *            the sum of divisors of k
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but guard against bogus calls */

    /* ---------- factorise n ---------- */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[32];
    int       exps[32];
    size_t    pcnt = 0;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* ---------- generate all divisors ---------- */
    size_t cap = 16;
    uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!divs) return 0;                 /* OOM: return 0 as sentinel */
    divs[0] = 1;
    size_t dcnt = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        int     e = exps[i];
        size_t old = dcnt;
        for (size_t j = 0; j < old; ++j) {
            seq_t val = divs[j];
            for (int k = 1; k <= e; ++k) {
                val *= p;
                if (dcnt == cap) {
                    cap <<= 1;
                    uint64_t *tmpp = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
                    if (!tmpp) { free(divs); return 0; }
                    divs = tmpp;
                }
                divs[dcnt++] = (uint64_t)val;
            }
        }
    }

    /* ---------- accumulate fraction ---------- */
    seq_t num = 0;
    seq_t den = 1;

    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        seq_t sigma = sigma_of((idx_t)d);

        seq_t g  = gcd(den, sigma);
        seq_t den_g  = den / g;
        seq_t sigma_g = sigma / g;

        /* new denominator: lcm(den, sigma) */
        seq_t new_den = den_g * sigma;

        /* numerator of the added term */
        seq_t term_num = (seq_t)d * den_g + num * sigma_g;

        /* reduce */
        seq_t g2 = gcd(term_num, new_den);
        num = term_num / g2;
        den = new_den   / g2;
    }

    free(divs);
    return den;
}
