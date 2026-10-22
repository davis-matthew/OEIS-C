/*
 * A386745  a(n) = n^2*sigma_2(n).
 * https://oeis.org/A386745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^2 * sigma_2(n) (sigma_2 = sum of squared divisors)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 239, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* by definition a(0)=0 */

    seq_t n_seq = (seq_t)n;        /* cast index to seq_t */
    seq_t sigma = sigma_k(n, 2);   /* sigma_2(n) */

    /* a(n) = n² * sigma_2(n) */
    seq_t res = n_seq;
    res *= n_seq;                  /* n² */
    res *= sigma;                  /* n² * sigma_2(n) */

    return res;
}
