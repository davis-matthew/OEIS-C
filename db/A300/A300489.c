/*
 * A300489  a(n) = n! * [x^n] -log(1 - x)/(1 - n*x).
 * https://oeis.org/A300489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} n^(n-k)/k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   factorial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* n! */
    seq_t fact = factorial(n);

    /* compute n^(n-1) – for k = 1 the exponent is n‑1 */
    seq_t pow = pow_seq((seq_t)n, n - 1);

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* term = (n! / k) * n^(n-k)  */
        seq_t term = (fact / (seq_t)k) * pow;
        sum += term;

        /* prepare pow for the next k (divide by n) */
        if (k != n)
            pow /= (seq_t)n;
    }

    return sum;
}
