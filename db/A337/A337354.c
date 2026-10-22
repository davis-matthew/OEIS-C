/*
 * A337354  a(n) is the numerator of Product_{i=0..n-1}
 *          (n-i)^((-1)^ceiling(i/2)).
 * https://oeis.org/A337354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Π_{i=0}^{n-1} (n-i)^{(-1)^⌈i/2⌉}
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 76, u128 n <= 152,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"


/* Computes the numerator of the product described in the OEIS entry for
   sequence A337354.  For n=0 the function returns 0.  The result is
   reduced to lowest terms at every step. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t num = 1;   /* current numerator (coprime to den) */
    seq_t den = 1;   /* current denominator (coprime to num) */

    for (idx_t i = 0; i < n; ++i) {
        seq_t m = (seq_t)(n - i);

        /* Sign pattern: + for i % 4 == 0 or 3, otherwise - */
        bool positive = ((i & 3) == 0) || ((i & 3) == 3);

        if (positive) {
            seq_t g = gcd(m, den);
            m /= g;
            den /= g;
            num *= m;
        } else {
            seq_t g = gcd(m, num);
            m /= g;
            num /= g;
            den *= m;
        }
    }
    return num;
}
