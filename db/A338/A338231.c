/*
 * A338231  Sum of the numbers less than or equal to n whose square does not
 *          divide n.
 * https://oeis.org/A338231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 – σ( Π p^{⌊e_p(n)/2⌋} )
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = sum of numbers ≤ n whose square does NOT divide n                     */
/* The implementation factors n, builds the product of prime powers taken   */
/* to the floor of half their exponents, and subtracts the sum of all divisors   */
/* of that product from the total sum 1+2+…+n.                                   */
/* This O(√n) solution is adequate for n up to ≈10 000 (and beyond)               */

#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Helper: compute 1 + p + p² + … + p^l  (σ(p^l) for integer l ≥ 0) */
/* ------------------------------------------------------------------ */
static seq_t sigma_prime_power(seq_t p, unsigned l)
{
    seq_t sum = 1;        /* the term p^0 */
    seq_t p_pow = 1;
    for (unsigned i = 0; i < l; ++i) {
        p_pow *= p;
        sum += p_pow;
    }
    return sum;
}

/* ------------------------------------------------------ */
/* Main function required by the test harness.            */
/* ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for non‑positive arguments */

    /* 1 + 2 + … + n  */
    seq_t total = (seq_t)n * ((seq_t)n + 1U) / 2U;

    /* Factor n and accumulate σ(Π p^{⌊e_p/2⌋}) */
    idx_t temp = n;
    seq_t sigma = 1U;                     /* product of σ(p^l) over all primes */

    /* trial division up to sqrt(temp) */
    idx_t p = 2;
    while (p <= temp / p) {
        if (temp % p == 0U) {
            unsigned exp = 0U;
            while (temp % p == 0U) {
                temp /= p;
                ++exp;
            }
            unsigned l = exp >> 1;        /* floor(exp/2) */
            if (l != 0U) {
                sigma *= sigma_prime_power((seq_t)p, l);
            }
        }
        ++p;
    }

    /* If a prime factor > sqrt(n) remains, it has exponent 1
       → floor(1/2) = 0, so it contributes nothing to σ.      */
    /* (No need to explicitly multiply by 1.) */

    /* Result: total sum minus the sum of eligible numbers.       */
    if (total < sigma) return 0;          /* should not happen for n ≥ 1 */
    return total - sigma;
}
