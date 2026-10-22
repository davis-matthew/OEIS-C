/*
 * A376887  The number of divisors of n that are products of factors of the
 *          form p^(k!) with multiplicities not larger than their multiplicity
 *          in n, where p is a prime and k >= 1, when the factorization of n
 *          is uniquely done using the factorial-base representation of the
 *          exponents in the prime factorization...
 * https://oeis.org/A376887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, base
 * Formula:   a(n) = ∏_{p^e || n} ∏_{i≥1} (digit_i(e) + 1) where e = Σ
 *            digit_i·i! with 0 ≤ digit_i ≤ i
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, factorial_digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts the divisors d of n whose exponents in the
   prime factorisation of d are admissible in the factorial‑base
   representation of the exponents of n. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/factorial_digit_product.h"

/* Main function for OEIS A376887 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, safety */

    /* Factor n.  The helper stores primes and exponents in arrays
       and gives the count in cnt. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t j = 0; j < cnt; ++j) {
        seq_t g = factorial_digit_product(exps[j]);
        result *= g;
    }
    return result;
}
