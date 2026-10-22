/*
 * A376885  The number of factors of n of the form p^(k!) counted with
 *          multiplicity, where p is a prime and k >= 1, when the
 *          factorization is uniquely done using the factorial-base
 *          representation of the exponents in the prime factorization of n.
 * https://oeis.org/A376885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum_{p^e || n} sum_{k>=1} d_k, where e = Σ d_k * k! is
 *            the factorial-base representation of the exponent of prime p in
 *            the prime factorization of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

#define MAX_FACTORS 64

static uint64_t primes_buf[MAX_FACTORS];
static uint64_t exps_buf[MAX_FACTORS];

static seq_t factorial_digit_sum(uint64_t e)
{
    seq_t sum = 0;
    for (uint64_t k = 1; e > 0; ++k) {
        uint64_t base = k + 1;           /* base for k! position */
        uint64_t digit = e % base;       /* digit in that position */
        sum += (seq_t)digit;             /* add digit to sum */
        e /= base;                       /* shift to next position */
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* n is at least 1; a(1)=0 by definition */
    if (n == 1)
        return 0;

    /* Factorize n */
    size_t cnt;
    factor_u64((uint64_t)n, primes_buf, exps_buf, &cnt);

    /* Sum factorial-base digits of all exponents */
    seq_t total = 0;
    for (size_t i = 0; i < cnt; ++i) {
        total += factorial_digit_sum(exps_buf[i]);
    }
    return total;
}
