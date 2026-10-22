/*
 * A388994  a(n) = {sum of squarefree divisors of n} minus {sum of divisors of
 *          n that have a square factor}.
 * https://oeis.org/A388994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*A048250(n) - A000203(n) ( = 2*sum of squarefree
 *            divisors - sum of all divisors )
 * Range:     i8 n <= 12, i16 n <= 2673, i32 n <= 9097, i64 n <= 9097,
 *            i128 n <= 9097, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, but safe */

    /* Factorise n, store primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t f_prod = 1;      /* product of (1 + p) over distinct primes (sum of squarefree divisors) */
    seq_t sigma_prod = 1;  /* product of σ(p^e) over distinct primes (sum of all divisors) */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* sum of squarefree divisors contribution: 1 + p */
        f_prod *= (seq_t)1 + p;

        /* sum of divisors contribution: (p^(e+1)-1)/(p-1) */
        seq_t p_pow = seq_pow(p, (idx_t)(e + 1));
        seq_t sigma_p = (p_pow - (seq_t)1) / (p - (seq_t)1);
        sigma_prod *= sigma_p;
    }

    /* a(n) = 2*f_prod - sigma_prod */
    seq_t result = (seq_t)2 * f_prod - sigma_prod;
    return result;
}
