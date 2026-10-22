/*
 * A339767  a(n) = 2*gpf(n) - Sum_{p | n, p prime} p*m(p), where gpf(n) =
 *          A006530(n) is the greatest prime factor of n and m(p) is the
 *          multiplicity of p in the prime factorization of n.
 * https://oeis.org/A339767
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*gpf(n) - Sum_{p|n} p*m(p)
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   factor_u64, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gpf.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 2 – return 0 to keep the function well‑defined. */
    if (n < 2u) return 0;

    uint64_t x = (uint64_t)n;

    /* Factorise x. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(x, primes, exps, &cnt);

    /* Compute the sum of p*m(p). */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum += (seq_t)primes[i] * (seq_t)exps[i];
    }

    /* Compute the result 2*gpf(x) - sum. */
    seq_t g = (seq_t)gpf(x);
    return 2 * g - sum;
}
