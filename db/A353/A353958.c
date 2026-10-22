/*
 * A353958  Sum of the divisors of n whose arithmetic derivative is even.
 * https://oeis.org/A353958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * ((1+d') mod 2) where d' is the arithmetic
 *            derivative. d' is even iff (i) the exponent of 2 in d is not
 *            exactly 1, and (ii) if that exponent is 0, the sum of the
 *            exponents of the odd primes in d is even. This is equivalent to
 *            skipping only those divisors d with v₂(d)=1, or wit...
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include the helper that factors n */
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Recursive helper to generate all divisors of n.
   `primes` and `exps` contain the prime factorisation of n.
   `idx`   is the current position in those arrays.
   `pcnt`  is the number of distinct prime factors.
   `cur`   is the divisor value built so far.
   `ex2`   is the exponent of 2 in the current divisor.
   `parity`=sum of exponents of odd primes for the current divisor modulo 2.
   `sum`   accumulates the total of those divisors whose
           arithmetic derivative is even. */
static void rec_div(const uint64_t primes[], const uint64_t exps[],
                    size_t idx, size_t pcnt,
                    seq_t cur, unsigned int ex2, unsigned int parity,
                    seq_t *sum)
{
    if (idx == pcnt) {
        if (ex2 == 1)          /* vanishing evenness */
            return;
        if (ex2 == 0) {
            if ((parity & 1) == 0)
                *sum += cur;
        } else
            *sum += cur;
        return;
    }

    uint64_t p   = primes[idx];
    uint64_t max = exps[idx];
    seq_t   val  = 1;

    for (uint64_t e = 0; e <= max; ++e) {
        unsigned int n_ex2     = ex2;
        unsigned int n_parity  = parity;
        if (p == 2)
            n_ex2 = (unsigned int)e;
        else
            n_parity ^= (unsigned int)(e & 1);

        rec_div(primes, exps, idx + 1, pcnt,
                cur * val, n_ex2, n_parity, sum);

        val *= (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    uint64_t num = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   pcnt  = 0;

    factor_u64(num, primes, exps, &pcnt);

    seq_t sum = 0;
    rec_div(primes, exps, 0, pcnt, (seq_t)1, 0, 0, &sum);
    return sum;
}
