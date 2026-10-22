/*
 * A378444  a(n) is the number of divisors d of n such that A083345(d) is
 *          even, where A083345(n) is the numerator of Sum(e/p:
 *          n=Product(p^e)).
 * https://oeis.org/A378444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2525, u16 n <= 9570, u32 n <= 9570, u64 n <= 9570,
 *            u128 n <= 9570, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A378444: a(n) = number of divisors d of n such that
   A083345(d) is even, where A083345(d) is the numerator of
   sum(e/p) for d = product(p^e). */

/* include standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* external helper for prime factorisation */
#include "../utils/factor_u64.h"

/* Compute g = gcd(a,b) for seq_t. */
static seq_t _gcd(seq_t a, seq_t b) {
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Integer power: base^exp (exp <= 64). */
static seq_t _pow_uint(uint64_t base, uint64_t exp) {
    seq_t r = (seq_t)1;
    while (exp) {
        if (exp & (uint64_t)1) r *= (seq_t)base;
        base *= base;
        exp >>= (uint64_t)1;
    }
    return r;
}

/* Return true if A083345(d) is even. */
static bool _is_A083345_even(uint64_t d) {
    if (d == 1) {
        /* numerator 0, even */
        return true;
    }

    /* factorisation of d */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(d, primes, exps, &cnt);

    seq_t denom = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) denom *= (seq_t)primes[i];

    seq_t num = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t factor = denom / (seq_t)primes[i];
        num += (seq_t)exps[i] * factor;
    }

    seq_t g = _gcd(num, denom);
    num /= g;

    return ((num & (seq_t)1) == (seq_t)0);
}

/* Main sequence function */
seq_t a(idx_t n) {
    uint64_t x = (uint64_t)n;
    if (x == 0) return (seq_t)0; /* undefined, but safe */

    /* factor n */
    uint64_t primes[64], exps[64];
    size_t num_factors;
    factor_u64(x, primes, exps, &num_factors);

    /* generate all divisors of n */
    size_t divs_cap = 16;
    seq_t *divs = (seq_t *)malloc(divs_cap * sizeof(seq_t));
    if (!divs) return (seq_t)0; /* allocation failure */
    size_t divs_len = 1;
    divs[0] = (seq_t)1;

    for (size_t pi = 0; pi < num_factors; ++pi) {
        uint64_t p = primes[pi];
        uint64_t e = exps[pi];
        size_t current_len = divs_len;
        seq_t mult = (seq_t)1;
        for (uint64_t ee = 1; ee <= e; ++ee) {
            mult *= (seq_t)p;
            for (size_t j = 0; j < current_len; ++j) {
                if (divs_len == divs_cap) {
                    divs_cap <<= 1;
                    divs = (seq_t *)realloc(divs, divs_cap * sizeof(seq_t));
                    if (!divs) return (seq_t)0; /* allocation failure */
                }
                divs[divs_len++] = divs[j] * mult;
            }
        }
    }

    /* count divisors with even A083345 */
    seq_t count = (seq_t)0;
    for (size_t i = 0; i < divs_len; ++i) {
        uint64_t d = (uint64_t)divs[i];
        if (_is_A083345_even(d)) count++;
    }

    free(divs);
    return count;
}
