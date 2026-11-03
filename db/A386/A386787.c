/*
 * A386787  a(n) = n^4*sigma_7(n).
 * https://oeis.org/A386787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^4 * sigma_7(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 56, u128 n <= 3181,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 7477 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute x^7 using only seq_t arithmetic */
static seq_t power7(seq_t x)
{
    seq_t r = 1;
    for (int i = 0; i < 7; ++i)
        r *= x;
    return r;
}

seq_t a(idx_t n)
{
    /* compute sigma_7(n) by enumerating divisors */
    seq_t sigma = 0;
    idx_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            seq_t d1 = (seq_t)i;
            sigma += power7(d1);
            idx_t j = n / i;
            if (j != i) {
                seq_t d2 = (seq_t)j;
                sigma += power7(d2);
            }
        }
    }

    /* compute n^4 */
    seq_t nseq = (seq_t)n;
    seq_t n2 = nseq * nseq;
    seq_t n4 = n2 * n2;

    return n4 * sigma;
}
