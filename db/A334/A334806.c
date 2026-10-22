/*
 * A334806  a(n) = Product_{d|n} lcm(tau(d), sigma(d)) where tau(k) is the
 *          number of divisors of k (A000005) and sigma(k) is the sum of
 *          divisors of k (A000203).
 * https://oeis.org/A334806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} lcm(tau(d), sigma(d))
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 35, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static uint64_t *spf = NULL;
static size_t   spf_size = 0;

/* Build an array of smallest prime factors up to 'limit' (inclusive). */
static void compute_spf(size_t limit)
{
    if (limit + 1 <= spf_size) return;
    uint64_t *new_spf = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
    for (size_t i = 0; i <= limit; ++i) new_spf[i] = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = i;
            if (i <= limit / i) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }
    free(spf);
    spf = new_spf;
    spf_size = limit + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    if (nn < 2) compute_spf(2);
    else        compute_spf(nn);

    /* Factor n */
    uint64_t fac_p[64];
    unsigned int fac_e[64];
    size_t fac_cnt = 0;
    uint64_t m = nn;
    while (m > 1) {
        uint64_t p = spf[m];
        unsigned int e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        fac_p[fac_cnt] = p;
        fac_e[fac_cnt] = e;
        ++fac_cnt;
    }

    /* Generate all divisors of n */
    uint64_t divs[256];
    size_t div_cnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = fac_p[i];
        unsigned int e = fac_e[i];
        size_t cur_cnt = div_cnt;
        for (size_t j = 0; j < cur_cnt; ++j) {
            uint64_t val = divs[j];
            for (unsigned int k = 1; k <= e; ++k) {
                val *= p;
                divs[div_cnt++] = val;
            }
        }
    }

    /* Compute product of lcm(tau(d), sigma(d)) over all divisors d */
    seq_t product = 1;
    for (size_t idx = 0; idx < div_cnt; ++idx) {
        uint64_t d = divs[idx];
        seq_t tau = 1;
        seq_t sigma = 1;
        m = d;
        while (m > 1) {
            uint64_t p = spf[m];
            unsigned int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            tau *= (seq_t)(e + 1);

            /* Compute p^(e+1) */
            seq_t pow = 1;
            for (unsigned int k = 0; k < e + 1; ++k) pow *= p;
            seq_t num = pow - 1;
            seq_t den = (seq_t)(p - 1);
            sigma *= num / den;
        }

        seq_t g = gcd(tau, sigma);
        seq_t l = tau / g;
        l *= sigma;
        product *= l;
    }
    return product;
}
