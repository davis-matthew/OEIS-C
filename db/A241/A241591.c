/*
 * A241591  Denominators of Postnikov's hook-length formula
 *          2^n*(n+1)^(n-1)/n!.
 * https://oeis.org/A241591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of 2^n * (n+1)^(n-1) / n! (fully reduced)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Computes v_p(m) – exponent of prime p in factorisation of m.
   m is given as idx_t, p is a small prime (uint64_t). */
static idx_t vp_in_value(idx_t m, uint64_t p) {
    idx_t cnt = 0;
    while (m % p == 0) {
        cnt++;
        m /= p;
    }
    return cnt;
}

static idx_t primes_limit = 0;
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;

seq_t a(idx_t n) {
    /* Handle trivial cases */
    if (n == 0 || n == 1) return (seq_t)1;

    /* Ensure we have the list of primes up to n */
    if (n > primes_limit) {
        free(primes);
        build_primes_upto(n, &primes, &primes_cnt);
        primes_limit = n;
    }

    seq_t result = (seq_t)1;
    uint64_t n_plus1 = (uint64_t)n + 1;

    for (size_t ip = 0; ip < primes_cnt; ++ip) {
        uint64_t p = primes[ip];
        if (p > n) break;

        /* Exponent of p in n! */
        idx_t e_den = 0;
        idx_t pw = p;
        while (pw <= n) {
            e_den += n / pw;
            if (pw > n / p) break;   /* avoid overflow for next power */
            pw *= p;
        }

        /* Exponent of p in numerator */
        idx_t vp = vp_in_value(n_plus1, p);
        idx_t e_num = 0;
        if (p == 2) {
            e_num = n + (n - 1) * vp;
        } else {
            e_num = (n - 1) * vp;
        }

        /* Remaining exponent to keep in denominator */
        if (e_den > e_num) {
            idx_t diff = e_den - e_num;
            seq_t pow_p = ipow_idx((seq_t)p, diff);
            result = seq_mul(result, pow_p);
        }
    }

    return result;
}
