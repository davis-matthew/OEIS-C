/*
 * A307911  Coefficient of x^n in expansion of (1 - n*x - n*x^2)^n.
 * https://oeis.org/A307911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (-n)^(n-k) * binomial(n,k) *
 *            binomial(n-k,k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    seq_t base   = -(seq_t)n;
    seq_t power  = seq_pow(base, n);          /* (-n)^n  (seq_pow handles n=0 as 1) */

    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        /* C(n,k) */
        seq_t bin1 = binomial(n, k);
        /* C(n-k,k) */
        seq_t bin2 = binomial(n - k, k);

        seq_t term = power;
        term *= bin1;
        term *= bin2;
        result += term;

        if (k < maxk) {
            /* Next power: (-n)^(n-(k+1)) = power / (-n) */
            power = power / base;              /* division exact */
        }
    }
    return result;
}
