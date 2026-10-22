/*
 * A357502  a(n) = ((1 + sqrt(n))^n - (1 - sqrt(n))^n)/(2*sqrt(n)).
 * https://oeis.org/A357502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..floor(n/2)} n^j * binomial(n, 2*j+1) This
 *            implements the OEIS sequence A357502 which satisfies a(n) = ((1
 *            + sqrt(n))^n - (1 - sqrt(n))^n)/(2*sqrt(n))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Include the helper that provides the binomial routine. */
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n == 0 we return 0. */
    if (n == 0) return 0;

    seq_t sum = 0;              /* running sum of the terms */
    seq_t n_pow = 1;            /* n^j, starting with j = 0 */
    seq_t n_seq = (seq_t)n;     /* cast index to seq_t */

    for (idx_t j = 0, k = 1; k <= n; ++j, k += 2) {
        seq_t bin = binomial(n, k);   /* C(n, k) */
        seq_t term = bin * n_pow;     /* n^j * C(n, k) */
        sum += term;
        n_pow *= n_seq;              /* advance n_pow = n^(j+1) */
    }
    return sum;
}
