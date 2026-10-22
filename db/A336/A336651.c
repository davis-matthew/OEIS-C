/*
 * A336651  Odd part of n divided by its largest squarefree divisor.
 * https://oeis.org/A336651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p odd, p|n} p^{v_p(n)-1} (i.e. odd part of n divided
 *            by its largest squarefree divisor), with a(2^e)=1 for all e
 * Range:     u8 n <= 2186, u16 n <= 9825, u32 n <= 9825, u64 n <= 9825,
 *            u128 n <= 9825, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>     /* for uint64_t */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t val = n;

    /* strip all factors of 2 – they contribute nothing. */
    while ((val & 1ULL) == 0 && val > 0)
        val >>= 1;      /* equivalent to val /= 2; */

    /* factorise the remaining odd part by trial division. */
    for (idx_t p = 3; p * p <= val; p += 2) {
        if (val % p == 0) {
            idx_t exp = 0;
            while (val % p == 0) {
                val /= p;
                exp++;
            }
            /* contribution from this prime: p^(exp-1) for exp>1, 1 otherwise. */
            if (exp > 1) {
                seq_t mult = 1;
                for (idx_t i = 0; i < exp - 1; ++i)
                    mult *= p;
                result *= mult;
            }
        }
    }

    /* any remaining factor > sqrt(val) is a prime with exponent 1,
       which contributes 1, so nothing to do. */
    return result;
}
