/*
 * A326990  Sum of odd divisors of n that are greater than 1.
 * https://oeis.org/A326990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of odd divisors of n that are greater than 1 The odd
 *            divisors of n are exactly the divisors of its odd part m =
 *            n/(2^k)
 * Range:     u8 n <= 164, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the sum of all divisors of a positive integer x.
   x is assumed to be <= UINT64_MAX.  The result may exceed 64 bits
   and is returned in type seq_t. */
static seq_t sigma_uint64(uint64_t x)
{
    seq_t result = (seq_t)1;   /* product of divisor sums for each prime factor */
    uint64_t temp = x;

    /* Trial division over potential prime factors */
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* Compute term = 1 + p + p^2 + ... + p^e for this prime factor */
            seq_t term = (seq_t)1;
            seq_t pow  = (seq_t)1;
            while (temp % p == 0) {
                temp /= p;
                pow *= (seq_t)p;  /* pow = p^i */
                term += pow;      /* accumulate divisor sum for this factor */
            }
            result *= term;       /* multiply into overall sum */
        }
    }

    /* If a prime factor remains > 1 after the loop */
    if (temp > 1) {
        /* term = 1 + temp */
        result *= ((seq_t)temp + (seq_t)1);
    }

    return result;
}

seq_t a(idx_t n)
{
    /* Strip powers of 2 to obtain the odd part */
    uint64_t m = (uint64_t)n;
    while ((m & 1ULL) == 0) {
        m >>= 1;
    }

    /* Sum of odd divisors is sigma(m); subtract 1 to exclude divisor 1 */
    seq_t sum_odds = sigma_uint64(m);
    sum_odds -= (seq_t)1;
    return sum_odds;
}
