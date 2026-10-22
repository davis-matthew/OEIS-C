/*
 * A330078  a(n) = denominator of Sum_{1 <= i < j <= d(n)} 1/(d_j - d_i), sum
 *          over ordered pairs of divisors of n, where d(n) is the number of
 *          divisors of n.
 * https://oeis.org/A330078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 47, u64 n <= 179, u128 n <= 659,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A function that returns the denominator of the sum
   of 1/(d_j - d_i) over all ordered pairs of distinct
   divisors of n (OEIS A330078).  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Factorise n ------------------------------------- */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[64];
    int      exps [64];
    size_t   pf   = 0;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pf] = p;
            exps [pf] = e;
            ++pf;
        }
    }
    if (tmp > 1) {              /* leftover prime factor */
        primes[pf] = tmp;
        exps [pf] = 1;
        ++pf;
    }

    /* Count total number of divisors ------------------- */
    size_t dcnt = 1;
    for (size_t i = 0; i < pf; ++i) {
        dcnt *= (size_t)(exps[i] + 1);
    }

    /* Generate all divisors into a simple uint64_t array */
    uint64_t *divs = (uint64_t *)malloc(dcnt * sizeof(uint64_t));
    if (!divs) return (seq_t)0;            /* unlikely, but safe */

    size_t idx = 0;
    divs[idx++] = 1;                       /* start with divisor 1 */

    for (size_t i = 0; i < pf; ++i) {
        size_t cur = idx;
        uint64_t base = primes[i];
        uint64_t pow  = 1;
        for (int e = 1; e <= exps[i]; ++e) {
            pow *= base;
            for (size_t j = 0; j < cur; ++j) {
                divs[idx++] = divs[j] * pow;
            }
        }
    }

    /* Sort the divisors --------------------------------- */
    for (size_t i = 1; i < idx; ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (divs[j] > divs[i]) {
                uint64_t tmp = divs[j];
                divs[j] = divs[i];
                divs[i] = tmp;
            }
        }
    }

    /* Special case: only one divisor (n = 1) ---------- */
    if (idx <= 1) {
        free(divs);
        return (seq_t)1;
    }

    /* Sum the reciprocals of all positive differences. */
    seq_t num = (seq_t)0;
    seq_t den = (seq_t)1;

    for (size_t i = 0; i < idx - 1; ++i) {
        for (size_t j = i + 1; j < idx; ++j) {
            seq_t diff = (seq_t)(divs[j] - divs[i]);   /* > 0          */
            seq_t g    = gcd(den, diff);               /* common part */
            seq_t t1   = diff / g;                     /* diff/g      */
            seq_t t2   = den  / g;                     /* den/g       */

            /* Add new fraction:  num/den + 1/diff */
            seq_t new_num = num * t1 + t2;
            seq_t new_den = den * t1;

            seq_t g2 = gcd(new_num, new_den);
            num = new_num / g2;
            den = new_den / g2;
        }
    }

    free(divs);
    return den;       /* result in reduced form               */
}
