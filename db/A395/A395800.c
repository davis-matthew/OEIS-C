/*
 * A395800  Expansion of -1/(Sum_{k>=0} binomial(5*k+3,k) * x^k).
 * https://oeis.org/A395800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/n) * (8*binomial(5*n-5,n-1) - 12*binomial(5*n-5,n-2))
 *            for n>0 a(0) = -1
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   _binom128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_binom128.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)(-1);

    /* compute binom(5*n-5, n-1) */
    idx_t ln1 = n - 1;
    unsigned __int128 c1 = _binom128(5 * n - 5, ln1);

    /* compute binom(5*n-5, n-2), if defined */
    unsigned __int128 c2 = 0;
    if (n > 1) {
        idx_t ln2 = n - 2;
        c2 = _binom128(5 * n - 5, ln2);
    }

    unsigned __int128 term1 = 8 * c1;
    unsigned __int128 term2 = 12 * c2;
    unsigned __int128 num = term1 - term2;
    unsigned __int128 denom = n;
    unsigned __int128 result = num / denom;  /* exact division */

    return (seq_t)result;
}
