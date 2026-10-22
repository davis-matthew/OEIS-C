/*
 * A351950  Arithmetic derivative of the factorial base exp-function: a(n) =
 *          A003415(A276076(n)).
 * https://oeis.org/A351950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A003415(A276076(n)) where A276076(n) = ∏_{k≥1}
 *            (prime_k)^{d_k}, with d_k the k‑th digit of n in factorial base
 *            (digit k = n mod (k+1) after successive division)
 * Range:     u8 n <= 16, u16 n <= 69, u32 n <= 476, u64 n <= 9958,
 *            u128 n <= 9882, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

static const seq_t PRIMES[] = {
    (seq_t)2,(seq_t)3,(seq_t)5,(seq_t)7,(seq_t)11,(seq_t)13,(seq_t)17,(seq_t)19,
    (seq_t)23,(seq_t)29,(seq_t)31,(seq_t)37,(seq_t)41,(seq_t)43,(seq_t)47,(seq_t)53,
    (seq_t)59,(seq_t)61,(seq_t)67,(seq_t)71,(seq_t)73,(seq_t)79,(seq_t)83,(seq_t)89
};

seq_t a(idx_t n)
{
    seq_t product = 1;   /* current value of A276076(n)       */
    seq_t deriv   = 0;   /* running arithmetic derivative    */
    idx_t k = 1;
    idx_t m = n;

    while (m > 0)
    {
        unsigned digit = (unsigned)(m % (k + 1));
        m /= (k + 1);

        if (digit)
        {
            seq_t p = PRIMES[k - 1];
            seq_t p_pow = seq_pow(p, (idx_t)digit);

            /* updated product after adding this prime factor */
            seq_t new_product = product * p_pow;

            /* contribution of this factor to the derivative */
            seq_t term = (new_product / p);   /* new_product divided by p   */
            term *= (seq_t)digit;

            deriv = deriv * p_pow + term;
            product = new_product;
        }
        ++k;
    }

    return deriv;
}
