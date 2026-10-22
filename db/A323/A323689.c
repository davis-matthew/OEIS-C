/*
 * A323689  G.f.: Sum_{n>=0} (x^n + i)^n / (1 + i*x^n)^(n+1), in which the
 *          constant term is taken to be 1.
 * https://oeis.org/A323689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in the power series expansion of
 *            Sum_{k>=1} (x^k + i)^k / (1 + i*x^k)^(k+1). The implementation
 *            computes this coefficient by summing over all positive divisors
 *            k of n and, for each k, over the admissible values of j. The
 *            contributions are combined as real integers; all i...
 * Range:     i8 n <= 11, i16 n <= 44, i32 n <= 167, i64 n <= 666,
 *            i128 n <= 2599, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

static void add_divisor_terms(idx_t k, idx_t n, seq_t *real_sum, seq_t *imag_sum)
{
    idx_t t = n / k;                     /* n = k * t   */
    idx_t maxj = (k < t) ? k : t;        /* j goes 0..maxj */

    for (idx_t j = 0; j <= maxj; ++j) {
        idx_t m = t - j;
        seq_t bin1 = binom(k, j);
        seq_t bin2 = binom(k + m, m);
        seq_t val = bin1 * bin2;        /* product fits into seq_t for needed n */

        idx_t s = k + t - 2 * j;        /* exponent of i in the factor */
        int sign = (m & 1) ? -1 : 1;    /* (-1)^m  */

        switch (s & 3) {                /* i^s : 0->1, 1->i, 2->-1, 3->-i */
            case 0: /* real factor +1 */
                if (sign == 1) *real_sum += val;
                else           *real_sum -= val;
                break;
            case 2: /* real factor -1 */
                if (sign == 1) *real_sum -= val;
                else           *real_sum += val;
                break;
            case 1: /* imag factor +i */
                if (sign == 1) *imag_sum += val;
                else           *imag_sum -= val;
                break;
            case 3: /* imag factor -i */
                if (sign == 1) *imag_sum -= val;
                else           *imag_sum += val;
                break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* constant term is defined to be 1 */

    seq_t real = 0;
    seq_t imag = 0;                      /* will be zero after accumulation */

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            add_divisor_terms(d, n, &real, &imag);
            if (d != n / d)
                add_divisor_terms(n / d, n, &real, &imag);
        }
    }

    /* imag should be zero; the sequence has real integer coefficients. */
    return real;
}
