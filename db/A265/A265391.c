/*
 * A265391  a(n) = numerator of Sum_{d|n} 1 / tau(d).
 * https://oeis.org/A265391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{d|n} 1 / tau(d). (OEIS A265391)
 *            Implementation: factor n, enumerate all divisors by backtracking
 *            over prime exponents, and add each 1 / tau(d) to a rational sum.
 *            The numerator of the reduced sum is returned. GCD reduction is
 *            performed after every addition
 * Range:     u8 n <= 31, u16 n <= 1023, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

/* Forward declaration of the recursive divisor enumeration */
static void dfs_divisors(size_t idx, size_t cnt, const uint64_t *ex,
                         seq_t tau, seq_t *num, seq_t *den);

/* Main sequence function */
seq_t a(idx_t n) {
    /* Use uint64_t for factorization input */
    uint64_t primes[24];
    uint64_t exponents[24];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exponents, &cnt);

    seq_t numerator = 0;  /* 0/1 initially */
    seq_t denominator = 1;

    /* Enumerate all divisors and accumulate 1 / tau(d) */
    dfs_divisors(0, cnt, exponents, (seq_t)1, &numerator, &denominator);

    return numerator;
}

/* Recursively enumerate all divisors based on prime exponents
   and update the running fraction sum. */
static void dfs_divisors(size_t idx, size_t cnt, const uint64_t *ex,
                         seq_t tau, seq_t *num, seq_t *den) {
    if (idx == cnt) {
        /* Add 1 / tau to the sum: num/den + 1/tau */
        seq_t new_num = (*num) * tau + (*den);
        seq_t new_den = (*den) * tau;
        seq_t g = gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;
        *num = new_num;
        *den = new_den;
        return;
    }

    /* Loop over all exponents for the current prime */
    for (size_t e = 0; e <= ex[idx]; ++e) {
        seq_t next_tau = tau * (seq_t)(e + 1);
        dfs_divisors(idx + 1, cnt, ex, next_tau, num, den);
    }
}
