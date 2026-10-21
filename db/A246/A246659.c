/*
 * A246659  a(n) = binomial(n-h,h)*hypergeometric([h-n/2,h-(n-1)/2],[1],4), h
 *          = floor(n/4).
 * https://oeis.org/A246659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n-h,h)*hypergeometric([h-n/2,h-(n-1)/2],[1],4),
 *            h = floor(n/4)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t h = n / 4;               /* floor(n/4)                                 */
    int r = (int)(n % 4);           /* n mod 4                                    */

    /* a_num and b_num hold the numerators of a and b in halves:
       a = a_num/2 ,  b = b_num/2                                     */
    int64_t a_num, b_num;
    idx_t K;                         /* number of terms in the hypergeometric sum  */

    switch (r) {
    case 0:                         /* n = 4h                                   */
        a_num = -(int64_t)(2 * h);  /* a = -h                                   */
        b_num = -(int64_t)(2 * h) + 1; /* b = -h + 1/2                           */
        K     = h;                  /* series terminates at k = h                 */
        break;
    case 1:                         /* n = 4h + 1                               */
        a_num = -(int64_t)(2 * h + 1); /* a = -h - 1/2                           */
        b_num = -(int64_t)(2 * h);     /* b = -h                                 */
        K     = h;                      /* series terminates at k = h                 */
        break;
    case 2:                         /* n = 4h + 2                               */
        a_num = -(int64_t)(2 * h + 2); /* a = -h - 1                              */
        b_num = -(int64_t)(2 * h + 1); /* b = -h - 1/2                            */
        K     = h + 1;                 /* series terminates at k = h + 1            */
        break;
    default:                        /* r == 3, n = 4h + 3                       */
        a_num = -(int64_t)(2 * h + 3); /* a = -h - 3/2                           */
        b_num = -(int64_t)(2 * h + 2); /* b = -h - 1                              */
        K     = h + 1;                 /* series terminates at k = h + 1            */
        break;
    }

    /* Compute the hypergeometric sum H = Σ_{k=0}^{K} (a)_k (b)_k 4^k / (k!)^2 */
    seq_t sum = 1;    /* term k = 0 contributes 1                        */
    seq_t term = 1;   /* current term t_{k-1}                            */

    for (idx_t k = 1; k <= K; ++k) {
        int64_t A = a_num + 2 * ((int64_t)k - 1);
        int64_t B = b_num + 2 * ((int64_t)k - 1);

        seq_t absA = (seq_t)(A >= 0 ? A : -A);
        seq_t absB = (seq_t)(B >= 0 ? B : -B);

        term = term * absA * absB;
        term = term / ((seq_t)k * (seq_t)k);   /* division is exact             */
        sum += term;
    }

    seq_t bin = binomial(n - h, h);   /* binomial (n-h, h)                     */
    return bin * sum;                 /* final result                           */
}
