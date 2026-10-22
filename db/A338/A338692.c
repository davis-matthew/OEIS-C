/*
 * A338692  Positions of 1's in A209615.
 * https://oeis.org/A338692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A343500(n)/2
 * Range:     u8 n <= 127, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the n-th integer x such that x = (2*k+1)*2^e with k+e even.
   This is equivalent to (x + 2*e) mod 4 == 1, where e is the exponent
   of 2 dividing x. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1; behaviour below unused */

    idx_t i = 1;            /* current candidate integer x */
    idx_t count = 0;        /* number of terms found so far */

    for (;; ++i) {
        idx_t t = i;        /* temporary copy for factoring */
        idx_t e = 0;        /* exponent of 2 in i */

        /* count trailing zeros to find e */
        while ((t & (idx_t)1) == 0) {
            ++e;
            t >>= 1;
        }

        /* t is now the odd part of i */
        if (((t + (2 * e)) & 3) == 1) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
    }
}
