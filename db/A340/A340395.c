/*
 * A340395  a(n) = A340131(A001006(n)).
 * https://oeis.org/A340395
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (5/8)*(3^n - (-1)^(n-1) - 2), offset 2. Recurrence
 *            (alternative): a(n+1) = 3*a(n) for even n ≥ 2; a(n+1) = 3*a(n)+5
 *            for odd n ≥ 3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* For the defined offset a(2) is the first term. */
    if (n < 2)
        return 0;                     /* undefined below offset */

    /* 3^n.  seq_pow works for arbitrary-precision seq_t as well. */
    seq_t pow3 = seq_pow((seq_t)3, n);

    /* delta = 1 if n is even, 3 if n is odd (see formula). */
    seq_t delta = (n & 1) ? (seq_t)3 : (seq_t)1;

    /* The quantity (pow3 - delta) is divisible by 8.  We divide first */
    /* to avoid overflow when multiplying by 5. */
    seq_t diff   = pow3 - delta;
    seq_t div8   = diff >> 3;          /* (pow3 - delta) / 8  */
    seq_t result = div8 * (seq_t)5;    /* a(n)                */

    return result;
}
