/*
 * A258122  The multiplicative Wiener index of the cycle graph C_n (n>=3).
 * https://oeis.org/A258122
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (k!)^n if n = 2k + 1 is odd; a(n) = k^k*((k-1)!)^n if n =
 *            2k is even
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   factorial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* Offset is 3 – behaviour for n < 3 does not matter */
    if ((n & 1) == 1) {           /* n is odd: n = 2k + 1  (k >= 1) */
        idx_t k = (n - 1) >> 1;
        seq_t fact_k = factorial(k);          /* k!  */
        return pow_seq(fact_k, (int)n);       /* (k!)^n  */
    } else {                       /* n is even: n = 2k  (k >= 2) */
        idx_t k = n >> 1;
        seq_t pow1 = pow_seq((seq_t)k, (int)k);       /* k^k  */
        seq_t fact_k1 = factorial(k - 1);            /* (k-1)!  */
        seq_t pow2 = pow_seq(fact_k1, (int)n);       /* ((k-1)!)^n  */
        return pow1 * pow2;                         /* product */
    }
}
