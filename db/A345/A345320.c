/*
 * A345320  Sum of the divisors of n whose square does not divide n.
 * https://oeis.org/A345320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Sum of the divisors of n whose square does not divide n
   Formula: a(n) = σ(n) – σ₂(n)
   where
      σ(n)   = ∏p (p^(e+1) – 1)/(p – 1)     (sum of all divisors)
      σ₂(n)  = ∏p (p^(⌊e/2⌋+1) – 1)/(p – 1) (sum of divisors d with d² | n)
   The implementation factors n by trial division up to √n and uses only
   integer arithmetic on type seq_t.                                                      */

#include <stdint.h>
#include <stdbool.h>

/* helper: power of a seq_t base to an integer exponent (exp ≥ 0) */
static seq_t pow_seq_uint(seq_t base, int exp)
{
    seq_t r = 1;
    while (exp-- > 0)
        r *= base;
    return r;
}

/* main function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* not required, but safe */
    uint64_t temp = (uint64_t)n;

    seq_t sigma = 1;          /* σ(n) */
    seq_t sigma_sq = 1;       /* σ₂(n) */

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p != 0)
            continue;
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        seq_t pseq = (seq_t)p;
        /* sum of divisors for this prime power */
        seq_t sigma_prime = (pow_seq_uint(pseq, exp + 1) - 1) / (pseq - 1);
        /* sum of divisors with d² | p^exp */
        seq_t sigma_sq_prime =
            (pow_seq_uint(pseq, (exp >> 1) + 1) - 1) / (pseq - 1);

        sigma *= sigma_prime;
        sigma_sq *= sigma_sq_prime;
    }

    if (temp > 1) {           /* remaining prime factor (exponent 1) */
        seq_t pseq = (seq_t)temp;
        seq_t sigma_prime = (pow_seq_uint(pseq, 2) - 1) / (pseq - 1); /* p+1 */
        /* exponent 1 ⇒ floor(1/2)=0 ⇒ sum = 1 */
        seq_t sigma_sq_prime = 1;

        sigma *= sigma_prime;
        sigma_sq *= sigma_sq_prime;
    }

    return sigma - sigma_sq;
}
