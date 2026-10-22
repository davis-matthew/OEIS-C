/*
 * A360120  a(n) = 1 if there are no solutions to k*n/(k+n) = x and k*n/(k-n)
 *          = y for integers x and y and natural number k, otherwise 0.
 * https://oeis.org/A360120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if there is no integer k>0 such that k*n/(k+n) and
 *            k*n/(k-n) are integers, otherwise 0. Equivalent test: for n>0,
 *            a(n)=0 if there exist two positive divisors d and f of n² such
 *            that |d−f| = 2·n (k > n) or d+f = 2·n (k < n); otherwise a(n)=1
 * Range:     u8 n <= 16, u16 n <= 8303, u32 n <= 8303, u64 n <= 8303,
 *            u128 n <= 8303, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside offset – undefined */
    if (n == 1) return 1;          /* special case to avoid
                                     unnecessary work */
    seq_t two_n = (seq_t)2 * (seq_t)n;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* generate all divisors of n²; max count is modest (≤4096) */
    seq_t divs[4096];
    size_t dlen = 1;
    divs[0] = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        seq_t p = (seq_t)primes[i];
        size_t all_exp = (size_t)(exps[i] * 2);  /* exponent in n² */
        size_t old_len = dlen;
        for (size_t j = 0; j < old_len; ++j) {
            seq_t base = divs[j];
            seq_t pow  = 1;
            for (size_t k = 1; k <= all_exp; ++k) {
                pow *= p;
                if (dlen >= sizeof(divs) / sizeof(divs[0])) /* safety */
                    return 1;
                divs[dlen++] = base * pow;
            }
        }
    }

    /* search for a valid pair */
    for (size_t i = 0; i < dlen; ++i) {
        for (size_t j = i + 1; j < dlen; ++j) {
            seq_t a1 = divs[i];
            seq_t a2 = divs[j];

            /* |a1-a2| == 2*n  (k > n)  */
            seq_t diff = a1 > a2 ? a1 - a2 : a2 - a1;
            if (diff == two_n) return (seq_t)0;

            /* a1 + a2 == 2*n  (k < n)  */
            if (a1 + a2 == two_n) return (seq_t)0;
        }
    }
    return (seq_t)1;
}
