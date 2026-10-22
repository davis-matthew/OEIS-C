/*
 * A346470  a(n) = psi(A276086(n)), where psi is Dedekind psi function
 *          A001615, and A276086 is the prime product form of primorial base
 *          expansion of n.
 * https://oeis.org/A346470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = psi(A276086(n))
 * Range:     u8 n <= 14, u16 n <= 56, u32 n <= 388, u64 n <= 2302,
 *            u128 n <= 2310, big any n
 * Helpers:   get_prime, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* psi(n) = n * Π_{p|n} (1 + 1/p) */
/* A276086(n) is written as Π_i p_i^{d_i} where (d_i) are the digits of n
   in the mixed-radix system with bases 2,3,5,7,… (prime bases). */

/* The function below computes the Dedekind ψ value directly from the
   digits, avoiding an explicit factorisation of A276086(n). */

#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k) */
#include "../utils/pow_int.h"     /* provides seq_t pow_int(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    idx_t temp   = n;
    seq_t result = 1;

    for (idx_t i = 1; temp > 0; ++i) {
        uint32_t prime = get_prime(i);          /* i‑th prime (2,3,5,…) */
        idx_t    base  = (idx_t)prime;
        idx_t    digit = temp % base;

        temp /= base;                           /* shift to next digit */

        if (digit == 0)
            continue;                           /* prime not present */

        /* For digit > 0: contribute p^{digit-1} * (p+1) to ψ */
        seq_t term = pow_int((seq_t)prime, (idx_t)(digit - 1));
        term *= (seq_t)(prime + 1);
        result *= term;
    }

    return result;
}
