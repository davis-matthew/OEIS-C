/*
 * A285021  Denominator of binomial(2*n,n)/20^n.
 * https://oeis.org/A285021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^(2*n - v2(binomial(2*n,n))) * 5^(n -
 *            v5(binomial(2*n,n))) = 20^n / gcd(binomial(2*n,n), 20^n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 695 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* count the exponent of prime p in n! */
static inline idx_t v_factorial(idx_t n, idx_t p)
{
    idx_t sum = 0;
    for (idx_t div = p; div <= n; /* increment inside */)
    {
        sum += n / div;
        if (div > n / p) break; /* prevent overflow */
        div *= p;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* exponents of 2 and 5 in binomial(2n,n) */
    idx_t v2 = v_factorial(2 * n, 2) - 2 * v_factorial(n, 2);
    idx_t v5 = v_factorial(2 * n, 5) - 2 * v_factorial(n, 5);

    /* exponents needed for the denominator */
    idx_t exp2 = 2 * n - v2;
    idx_t exp5 = n - v5;

    seq_t val = pow2(exp2);
    val = seq_mul(val, power5(exp5));

    return val;
}
