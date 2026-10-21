/*
 * A245906  Numbers of the form 4n^2 + 1 or 4n^2 + 8n + 1.
 * https://oeis.org/A245906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   if n is odd, let k = (n+1)/2 and a(n) = 4*k^2 + 1; if n is even,
 *            let k = n/2 and a(n) = 4*k^2 + 8*k + 1
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is >= 1 by sequence definition. */
    seq_t k;
    if ((n & 1ULL) == 0) {          /* even n */
        k = (seq_t)(n / 2);
        return 4 * k * k + 8 * k + 1;
    } else {                        /* odd n */
        k = (seq_t)(n / 2 + 1);     /* (n+1)/2 */
        return 4 * k * k + 1;
    }
}
