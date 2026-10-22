/*
 * A377985  a(n) = 2*n - phi(A003961(n)), where phi is Euler totient function
 *          and A003961 is fully multiplicative function with a(prime(i)) =
 *          prime(i+1).
 * https://oeis.org/A377985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*n - phi(A003961(n))
 * Range:     i8 n <= 63, i16 n <= 1023, i32 n <= 9786, i64 n <= 9786,
 *            i128 n <= 9786, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

/*                with A003961(p_i^e_i) = next_prime(p_i)^e_i  */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety, offset is 1 */
    seq_t phi_product = (seq_t)1;

    /* factorization of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t factors = 0;
    factor_u64((uint64_t)n, primes, exps, &factors);

    for (size_t i = 0; i < factors; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t next_p = next_prime(p);          /* A003961 maps p -> next prime */
        seq_t r = (seq_t)next_p;

        seq_t term;
        if (e == 1)
        {
            term = r - (seq_t)1;
        }
        else
        {
            seq_t pow_val = (seq_t)1;
            for (uint64_t k = 1; k < e; ++k)
                pow_val *= r;
            term = pow_val * (r - (seq_t)1);
        }
        phi_product *= term;
    }

    seq_t result = ((seq_t)n) * (seq_t)2 - phi_product;
    return result;
}
