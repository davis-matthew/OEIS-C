/*
 * A342394  a(n) = Sum_{k=1..n} k^(gcd(k,n) - 1).
 * https://oeis.org/A342394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} k^(gcd(k,n) - 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    /* n >= 1 by definition of the sequence */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t g = gcd((seq_t)k, (seq_t)n);   /* gcd of k and n */
        seq_t e = g - 1;                     /* exponent */

        seq_t term;
        if (e == 0)         /* k^0 = 1 */
            term = 1;
        else if (e == 1)    /* k^1 = k */
            term = (seq_t)k;
        else                /* general exponent */
            term = big_pow((seq_t)k, e);

        sum += term;
    }
    return sum;
}
