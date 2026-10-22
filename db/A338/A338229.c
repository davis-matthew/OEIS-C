/*
 * A338229  Number of ternary strings of length n that contain at least one 0
 *          and at most two 1's.
 * https://oeis.org/A338229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + n*2^(n-1) + binomial(n,2)*2^(n-2) - binomial(n,2) -
 *            n - 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = pow2(n);                   /* 2^n */
    seq_t n_seq  = (seq_t)n;                  /* n as seq_t */
    result += n_seq * pow2(n - 1);           /* n*2^(n-1) */

    seq_t binom2 = binomial(n, 2);            /* C(n,2) */
    if (n >= 2) {
        result += binom2 * pow2(n - 2);       /* binom*2^(n-2) */
    }

    result -= binom2;                        /* - C(n,2) */
    result -= n_seq;                         /* - n */
    result -= 1;                             /* - 1 */

    return result;
}
