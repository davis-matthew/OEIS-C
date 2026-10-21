/*
 * A295295  Sum of squarefree divisors of the powerful part of n: a(n) =
 *          A048250(A057521(n)).
 * https://oeis.org/A295295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} (p+1) for e>1 (otherwise 1)
 * Range:     u8 n <= 9102, u16 n <= 9102, u32 n <= 9102, u64 n <= 9102,
 *            u128 n <= 9102, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

seq_t a(idx_t n)
{
    /* Static cache of smallest prime factors up to the maximum n seen so far */
    static idx_t *spf = NULL;
    static idx_t spf_limit = 0;

    /* Expand the cache if needed */
    if (n > spf_limit) {
        if (spf) free(spf);
        build_spf(n, &spf);          /* spf[i] = smallest prime factor of i (0 for i≤1) */
        spf_limit = n;
    }

    seq_t result = (seq_t)1;
    idx_t temp = n;

    while (temp > 1) {
        idx_t p = spf[temp];
        if (p == 0) p = temp;        /* number is prime if spf not set */

        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }

        if (exp >= 2) {
            result = result * ((seq_t)(p + 1));
        }
    }

    return result;
}
