/*
 * A358839  Dirichlet inverse of A353627, the characteristic function of the
 *          squarefree numbers multiplied by binary powers.
 * https://oeis.org/A358839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 9749, i16 n <= 9749, i32 n <= 9749, i64 n <= 9749,
 *            i128 n <= 9749, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Dirichlet inverse of the characteristic function of squarefree numbers
 * multiplied by binary powers (OEIS A358839).
 *
 * Multiplicative rule:
 *   a(1)          =  1
 *   a(2^e)        = -1  if e = 1,   0  if e >= 2
 *   a(p^e)        = (-1)^e  for odd primes p
 *
 * For a general n = 2^k * m (m odd):
 *   if k >= 2            -> a(n) =  0
 *   else                 -> a(n) = (-1)^(k + Σ_i e_i) ,
 *     where the sum runs over the exponents e_i in the prime factorisation
 *     of the odd part m.
 *
 * The function factors n on the fly; the computation is O(√n) and
 * requires only the standard C integer types.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)                /* a(1) = 1, n = 0 is unused but safe */
        return (seq_t)1;

    idx_t tmp = n;
    int k = 0;                 /* 2‑adic valuation of n */
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++k;
        if (k >= 2)            /* any higher power of 2 kills the value */
            return (seq_t)0;
    }

    int parity = 0;            /* parity of Σ_e e for odd primes */

    /* factorise the odd part */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            parity ^= (e & 1); /* add parity of this exponent */
        }
    }
    if (tmp > 1)               /* remaining prime factor with exponent 1 */
        parity ^= 1;

    /* a(n) = (-1)^(k + parity) */
    return ((k + parity) & 1) ? (seq_t)-1 : (seq_t)1;
}
