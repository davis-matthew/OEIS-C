/*
 * A358648  Number of preference profiles of the stable roommates problem with
 *          2n participants.
 * https://oeis.org/A358648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2n-1)!)^(2n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n = 0, but return 0 */
    idx_t m   = 2 * n - 1;           /* 2n-1 */
    seq_t base = factorial(m);      /* (2n-1)! */
    seq_t exp  = (seq_t)(2 * n);     /* 2n as exponent */
    return big_pow(base, exp);
}
