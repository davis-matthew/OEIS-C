/*
 * A384354  Numbers k such that the arithmetic mean of the divisors of k
 *          evenly divides k+1.
 * https://oeis.org/A384354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n‑th integer k such that the arithmetic mean of its
   divisors evenly divides k+1. */

#include <stdint.h>
#include <stdbool.h>

/* Test whether a given k satisfies the condition.
   Return true if (sum_of_divisors(k) / number_of_divisors(k)) divides k+1. */
static bool good_k(uint64_t k)
{
    uint64_t temp = k;
    __uint128_t sigma = 1;   /* product of (p^(e+1)-1)/(p-1) for each prime factor p^e */
    __uint128_t d     = 1;   /* product of (e+1) – number of divisors */

    /* Trial division to factor k.  Use 128‑bit intermediate to avoid overflow
       when computing large powers. */
    uint64_t p = 2;
    while (p * p <= temp) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            __uint128_t pk  = 1;      /* p^exp */
            __uint128_t s   = 1;      /* 1 + p + … + p^exp */

            while (temp % p == 0) {
                temp /= p;
                ++exp;
                pk *= p;              /* p^exp */
                s += pk;              /* add new term to geometric series */
            }
            sigma *= s;
            d     *= (exp + 1);
        }
        ++p;
    }

    if (temp > 1) {                /* remaining prime factor */
        sigma *= (__uint128_t)(1 + temp);
        d     *= 2;
    }

    /* Check divisibility: (k+1) * d  is a multiple of sigma. */
    __uint128_t lhs = (__uint128_t)(k + 1) * d;
    return (lhs % sigma) == 0;
}

/* Return the n-th term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;    /* undefined below offset */

    uint64_t counter = 0;
    uint64_t k = 1;
    while (counter < n) {
        if (good_k(k)) {
            ++counter;
        }
        ++k;
    }
    return (seq_t)(k - 1);   /* k was incremented after the last found term */
}
