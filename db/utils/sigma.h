/* sigma: the sum of the divisors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_SIGMA_H
#define OEISC_UTILS_SIGMA_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: compute σ(k) for a 64‑bit integer k. This uses a simple
   trial‑division factorisation that is fast enough for the sizes
   required by the tests. */
static seq_t sigma(idx_t k) {
    idx_t n = k;
    seq_t sum = 1;               /* product of (p^(e+1)-1)/(p-1) for each prime factor */
    idx_t d = 2;
    while ((seq_t)d * (seq_t)d <= (seq_t)n) {
        if (n % d == 0) {
            seq_t term = 1;      /* 1 + d + d^2 + ... + d^e */
            seq_t power = 1;
            while (n % d == 0) {
                n /= d;
                power *= d;
                term += power;
            }
            sum *= term;
        }
        d = (d == 2 ? 3 : d + 2);   /* 2, then odd numbers only */
    }
    if (n > 1) {
        /* remaining prime factor exponent = 1 */
        sum *= ((seq_t)n + 1);
    }
    return sum;
}

#endif
