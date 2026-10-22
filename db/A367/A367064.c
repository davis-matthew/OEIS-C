/*
 * A367064  Determinant of the matrix d*e/gcd(d, e)^2, where d, e run through
 *          the unitary divisors of n.
 * https://oeis.org/A367064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∏_{p^e‖n} (1 - p^{2e})^{2^{ω(n)-1}}, where ω(n) is the
 *            number of distinct prime divisors of n. For n = 1, a(1) = 1.
 *            This computation follows the closed form derived in the OEIS
 *            entry
 * Range:     i8 n <= 5, i16 n <= 14, i32 n <= 29, i64 n <= 209, i128 n <= 329,
 *            big any n
 * Helpers:   big_pow, factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n,
                                     uint64_t *pr, uint64_t *ex,
                                     size_t *cnt) */
#include "../utils/seq_pow.h"     /* seq_pow(seq_t base, idx_t exp) */
#include "../utils/big_pow.h"     /* big_pow(seq_t base, seq_t exp) */

/* External linkage by requirement */
seq_t a(idx_t n)
{
    if (n == 1)  /* a(1) = 1 by definition */
        return (seq_t)1;

    /* Factor n into primes with their exponents.  */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    idx_t omega = cnt;                 /* distinct prime count */

    /* Compute product of |1 - p^{2e}| = p^{2e} - 1 for all primes. */
    seq_t abs_product = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t pow_val = seq_pow(primes[i], 2 * exps[i]); /* p^{2e} */
        seq_t factor = pow_val - 1;                     /* |1 - p^{2e}| */
        abs_product *= factor;                          /* accumulate */
    }

    /* Exponent is 2^{ω-1}.  For ω=1 it equals 1. */
    idx_t exponent = (idx_t)1 << (omega - 1);
    seq_t result = big_pow(abs_product, (seq_t)exponent);

    /* Sign: negative only when ω = 1 (prime power). */
    if (omega == 1)
        result = -result;

    return result;
}
