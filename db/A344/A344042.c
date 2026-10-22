/*
 * A344042  a(n) = n * Sum_{d|n} sigma(d)^2 / d.
 * https://oeis.org/A344042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * Sum_{d|n} sigma(d)^2 / d
 * Range:     u8 n <= 7, u16 n <= 71, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"

seq_t a(idx_t n)
{
    /* Prepare smallest‑prime‑factor table */
    static idx_t *spf = NULL;
    static idx_t spf_limit = 0;
    if (n > spf_limit) {
        if (spf != NULL) free(spf);
        build_spf(n, &spf);
        spf_limit = n;
    }

    /* Factorise n into primes */
    const int MAX_FACTORS = 16;
    uint64_t primes[MAX_FACTORS];
    int expns[MAX_FACTORS];
    int fcnt = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        uint64_t p = (uint64_t)spf[tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            e++;
        }
        primes[fcnt] = p;
        expns[fcnt] = e;
        fcnt++;
    }

    /* Enumerate all divisors of n and their sigma values */
    const int MAX_DIVISORS = 512;
    uint64_t divs[MAX_DIVISORS];
    seq_t sigmas[MAX_DIVISORS];
    int divcnt = 1;
    divs[0] = 1;
    sigmas[0] = 1;                     /* sigma(1) = 1 */

    for (int idx = 0; idx < fcnt; idx++) {
        uint64_t p = primes[idx];
        int      e = expns[idx];

        uint64_t  new_divs[MAX_DIVISORS];
        seq_t     new_sigmas[MAX_DIVISORS];
        int new_cnt = 0;

        for (int d = 0; d < divcnt; d++) {
            uint64_t baseDiv = divs[d];
            seq_t   baseSigma = sigmas[d];

            seq_t pow = 1;            /* p^k */
            seq_t sigma = 1;          /* sigma(p^k) */

            for (int k = 0; k <= e; k++) {
                if (k > 0) {
                    pow *= (seq_t)p;
                    sigma += pow;       /* sigma(p^k) = 1 + p + ... + p^k */
                }
                new_divs[new_cnt]  = baseDiv * (uint64_t)pow;
                new_sigmas[new_cnt] = baseSigma * sigma;
                new_cnt++;
            }
        }

        /* copy new arrays back into main ones */
        divcnt = new_cnt;
        for (int i = 0; i < divcnt; i++) {
            divs[i] = new_divs[i];
            sigmas[i] = new_sigmas[i];
        }
    }

    /* Compute the required sum */
    seq_t sum = 0;
    for (int i = 0; i < divcnt; i++) {
        uint64_t d = divs[i];
        seq_t sigma_d = sigmas[i];
        seq_t term = (seq_t)(n / d) * sigma_d * sigma_d;
        sum += term;
    }

    return sum;
}
