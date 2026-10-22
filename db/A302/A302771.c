/*
 * A302771  If there is Gaussian integer z such that the norm of z is n, a(n)
 *          is the absolute value of Product_{the norm of z is n} z. Otherwise
 *          a(n) = 0.
 * https://oeis.org/A302771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(R_2(n)/2), where R_2(n)=0 if any prime ≡3(mod4) has
 *            odd exponent; otherwise R_2(n)=4⋅∏_{p≡1(mod4)}(e_p+1)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 49, u64 n <= 259,
 *            u128 n <= 1104, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper utilities */
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Factor n */
    uint64_t pr[20], ex[20];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    /* Check for odd exponents of primes ≡3 (mod 4) */
    for (size_t i = 0; i < cnt; ++i) {
        if ((pr[i] & 3) == 3 && (ex[i] & 1) == 1) {
            return 0;
        }
    }

    /* Compute product over primes ≡1 (mod 4) of (exponent+1) */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if ((pr[i] & 3) == 1) {
            prod *= (seq_t)(ex[i] + 1);
        }
    }

    /* Exponent of n in the result is 2*prod */
    seq_t exp  = prod * 2;
    seq_t base = (seq_t)n;

    return big_pow(base, exp);
}
