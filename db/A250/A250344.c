/*
 * A250344  Numerator of the harmonic mean of the first n hexagonal numbers.
 * https://oeis.org/A250344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator of n / ( sum_{k=1}^n 1/(k(2k-1)) ) * in lowest
 *            terms. The function reduces the fraction * n / S, where S = sum
 *            1/hk, and returns the reduced numerator
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                     /* undefined for n < 1, return 0 */

    /* fraction S = sum_{k=1}^n 1/hk = sum_num / sum_den,  reduced each step */
    seq_t sum_num = (seq_t)0;
    seq_t sum_den = (seq_t)1;

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        /* hk = k(2k-1) */
        seq_t kk   = (seq_t)k;
        seq_t hk   = kk * ((kk << 1) - (seq_t)1);   /* hk = k*(2k-1) */

        /* new fraction: sum_num/sum_den + 1/hk = (sum_num*hk + sum_den) / (sum_den*hk) */
        seq_t new_num = sum_num * hk + sum_den;
        seq_t new_den = sum_den * hk;

        /* reduce */
        seq_t g = seq_gcd(new_num, new_den);
        if (g != (seq_t)1) {
            new_num /= g;
            new_den /= g;
        }

        sum_num = new_num;
        sum_den = new_den;
    }

    /* harmonic mean HM = n / sum = n * sum_den / sum_num */
    seq_t hm_num = (seq_t)n * sum_den;
    seq_t hm_den = sum_num;

    /* reduce numerator of HM */
    seq_t g = seq_gcd(hm_num, hm_den);
    if (g != (seq_t)1) {
        hm_num /= g;
        /* hm_den /= g;  not needed for the answer */
    }

    return hm_num;                     /* return a(n) */
}
