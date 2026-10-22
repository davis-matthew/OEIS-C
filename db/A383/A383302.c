/*
 * A383302  a(n) = 1 if A276086(A276086(n)) is a multiple of
 *          A276086(A003415(n)), otherwise 0, where A003415 is the arithmetic
 *          derivative, and A276086 is the primorial base exp-function.
 * https://oeis.org/A383302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A276086(A276086(n)) is a multiple of
 *            A276086(A003415(n)), 0 otherwise. The implementation compares
 *            the exponent vectors of the two numbers directly, avoiding
 *            overflow of large intermediate values
 * Range:     u8 n <= 19, u16 n <= 86, u32 n <= 5402, u64 n <= 9978,
 *            u128 n <= 9978, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/pow_seq.h"

/* first thirty primes; enough for primorial representations up to 2^64 */
static const uint64_t primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113
};
static const size_t NUM_PRIMES = sizeof(primes) / sizeof(primes[0]);

/* compute primorial‑base representation of a value (seq_t) and store digits.
   digits[i] holds the i‑th digit, i.e. exponent of prime primes[i].  */
static size_t primorial_rep_seq(seq_t val, seq_t digits[NUM_PRIMES])
{
    seq_t prim = 1;
    size_t k = 0;
    while (prim <= val && k < NUM_PRIMES) {
        seq_t prime_small = (seq_t)primes[k];
        seq_t digit = (val / prim) % prime_small;
        digits[k] = digit;
        /* advance prim = prim * prime_small, but stop if it would overflow */
        if (prim > val / prime_small) { ++k; break; }
        prim *= prime_small;
        ++k;
    }
    return k;  /* number of digits stored */
}

/* build the primorial value from its digit vector.  */
static seq_t primorial_from_digits(const seq_t digits[NUM_PRIMES], size_t len)
{
    seq_t prod = 1;
    for (size_t k = 0; k < len; ++k) {
        seq_t d = digits[k];
        if (d > 0)
            prod *= pow_seq((seq_t)primes[k], (int)d);
    }
    return prod;
}

/* arithmetic derivative A003415(n). Works for n up to 64‑bit. */
static seq_t arithmetic_derivative(idx_t n)
{
    if (n <= 1) return 0;
    seq_t d   = 0;
    idx_t   t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            unsigned int e = 0;
            while (t % p == 0) { ++e; t /= p; }
            d += (seq_t)e * (seq_t)(n / p);
        }
    }
    if (t > 1) d += (seq_t)(n / t);   /* remaining prime factor */
    return d;
}

/* Main sequence definition. */
seq_t a(idx_t n)
{
    /* n → e1 = A276086(n) */
    seq_t digits_n[NUM_PRIMES];
    size_t len_n = primorial_rep_seq((seq_t)n, digits_n);
    seq_t e1 = primorial_from_digits(digits_n, len_n);

    /* e1 → e2 = A276086(e1) : exponent vector of e2 is digits of e1 */
    seq_t digits_e1[NUM_PRIMES];
    size_t len_e1 = primorial_rep_seq(e1, digits_e1);

    /* n → d = A003415(n) → e3 = A276086(d) */
    seq_t d = arithmetic_derivative(n);
    seq_t digits_d[NUM_PRIMES];
    size_t len_d = primorial_rep_seq(d, digits_d);

    /* check divisibility: e3 divides e2 iff all exponents are ≤ */
    size_t maxlen = (len_e1 > len_d) ? len_e1 : len_d;
    for (size_t i = 0; i < maxlen; ++i) {
        seq_t exp_e3 = (i < len_d) ? digits_d[i] : 0;
        seq_t exp_e2 = (i < len_e1) ? digits_e1[i] : 0;
        if (exp_e3 > exp_e2) return 0;
    }
    return 1;
}
