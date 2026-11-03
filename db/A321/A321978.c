/*
 * A321978  9-dimensional Catalan numbers.
 * https://oeis.org/A321978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 118 (2026-11-01)
 * Helpers:   build_primes, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 9-dimensional Catalan numbers – formula:
   a(n) = 0!*1!*…*8! * (9n)! / (n!* (n+1)! * … * (n+8)!) */

#include <stdbool.h>
#include <stddef.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes.h"
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Legendre's formula – exponent of p in m! */
static idx_t legendre(idx_t m, idx_t p) {
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

/* Prime exponents in 0!*1!*…*8!  (i.e. C = 5056584744960000) */
static const idx_t SMALL_PRIMES[4]   = { 2, 3, 5, 7 };
static const idx_t SMALL_EXPLO[4]    = {23, 9, 4, 2};

/* Main sequence function */
seq_t a(idx_t n) {
    /* 9*n fits easily into idx_t for n up to 10000 */
    idx_t limit = n * 9ULL;

    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(limit, &primes, &prime_cnt);

    seq_t result = (seq_t)1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];

        /* exponent: C + (9n)! - Σ (n+i)!  */
        idx_t exp = legendre(limit, p);
        for (idx_t i = 0; i <= 8; ++i) {
            exp -= legendre(n + i, p);
        }

        /* add constant C's prime exponents */
        if (p == 2)      exp += 23;
        else if (p == 3) exp += 9;
        else if (p == 5) exp += 4;
        else if (p == 7) exp += 2;

        if (exp > 0) {
            seq_t tmp = ipow((seq_t)p, (seq_t)exp);
            result = seq_mul(result, tmp);
        }
    }

    return result;
}
