/*
 * A322673  a(n) = numerator of Product_{d|n} (sigma(d)/d) where sigma(k) =
 *          the sum of the divisors of k (A000203).
 * https://oeis.org/A322673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of product_{d|n} sigma(d)/d
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 99, u64 n <= 399,
 *            u128 n <= 1799, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses product over divisors of n of sigma(d)/d reduced to lowest terms. */

#include <stdlib.h>
#include <stdint.h>    /* for uint64_t */
#include <stdbool.h>
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

/* ------------------------------------------------------------------ */
/* Helper: compute sigma(n) (sum of divisors of n) */
static seq_t sigma_of(uint64_t n) {
    if (n == 1) return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        /* compute (p^(e+1)-1)/(p-1) */
        seq_t pow = (seq_t)1;
        for (uint64_t k = 0; k < e + 1; ++k) pow *= (seq_t)p;
        seq_t term = (pow - (seq_t)1) / ((seq_t)p - (seq_t)1);
        result *= term;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Helper: generate all divisors of n and store them in an array */
/* Returns number of divisors via out parameter 'size' */
static uint64_t *divisors_of(uint64_t n, size_t *size) {
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    /* compute total number of divisors */
    size_t total = 1;
    for (size_t i = 0; i < cnt; ++i) total *= (size_t)(exps[i] + 1);

    uint64_t *divs = (uint64_t *)malloc(total * sizeof(uint64_t));
    if (!divs) return NULL; /* allocation failure */

    size_t dcnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t old = dcnt;
        uint64_t mult;
        for (size_t j = 0; j < old; ++j) {
            mult = divs[j];
            for (uint64_t k = 1; k <= e; ++k) {
                mult *= p;
                divs[dcnt++] = mult;
            }
        }
    }

    *size = dcnt;
    return divs;
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined but safe */

    /* Build array of divisors */
    size_t dsize;
    uint64_t *divs = divisors_of((uint64_t)n, &dsize);
    if (!divs) return (seq_t)0; /* allocation failure */

    seq_t num = (seq_t)1;
    seq_t den = (seq_t)1;

    for (size_t i = 0; i < dsize; ++i) {
        uint64_t d = divs[i];
        seq_t sigma_d = sigma_of(d);

        num *= sigma_d;
        den *= (seq_t)d;

        seq_t g = gcd(num, den);
        if (g > (seq_t)1) {
            num /= g;
            den /= g;
        }
    }

    free(divs);
    return num;   /* numerator in lowest terms */
}
