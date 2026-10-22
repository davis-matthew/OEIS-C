/*
 * A332627  a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n,k) * k! * k^n.
 * https://oeis.org/A332627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n,k) * k! * k^n (OEIS
 *            sequence A332627)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Special case n = 0: term is defined as 1. */
    if (n == 0) {
        return 1;
    }

    seq_t pos = 0;   /* sum of terms with positive sign */
    seq_t neg = 0;   /* sum of terms with negative sign */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin   = binomial(n, k);          /* C(n,k) */
        seq_t factk = factorial(k);            /* k!   */
        seq_t powk  = seq_pow((seq_t)k, n);    /* k^n  */

        seq_t term = bin * factk * powk;        /* binom * k! * k^n */

        /* Sign: (-1)^(n-k) */
        if (((n - k) & 1) == 0) {
            pos += term;   /* positive */
        } else {
            neg += term;   /* negative */
        }
    }

    /* Final result: positive sum minus negative sum. */
    return pos - neg;
}
