/*
 * A329380  a(n) = Product_{d|n} A276086(d)^A001221(n/d).
 * https://oeis.org/A329380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} A276086(d)^A001221(n/d)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 29, u64 n <= 59, u128 n <= 167,
 *            big any n
 * Validated: standard, pass, 73 terms through n = 8141 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const unsigned A329380_pr[26] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101};

/* A276086: digits of d in primorial base become exponents of successive primes.
   Digit k has radix prime(k+1) and becomes the exponent of prime(k+1). */
static seq_t A329380_A276086(idx_t d)
{
    seq_t res = 1;
    idx_t m = d;
    int k = 0;
    while (m > 0 && k < 26) {
        idx_t radix = (idx_t)A329380_pr[k];
        idx_t dig = m % radix;
        m /= radix;
        for (idx_t j = 0; j < dig; ++j)
            res *= (seq_t)A329380_pr[k];
        ++k;
    }
    return res;
}

/* A001221: number of distinct prime divisors */
static unsigned A329380_omega(idx_t n)
{
    unsigned c = 0;
    if (n <= 1) return 0;
    if (n % 2 == 0) {
        ++c;
        while (n % 2 == 0) n /= 2;
    }
    for (idx_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            ++c;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) ++c;
    return c;
}

static seq_t A329380_term(idx_t d, idx_t n)
{
    unsigned w = A329380_omega(n / d);
    seq_t r = 1;
    if (w == 0) return r;
    seq_t b = A329380_A276086(d);
    for (unsigned j = 0; j < w; ++j)
        r *= b;
    return r;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0) continue;
        idx_t j = n / i;
        result *= A329380_term(i, n);
        if (j != i)
            result *= A329380_term(j, n);
    }
    return result;
}
