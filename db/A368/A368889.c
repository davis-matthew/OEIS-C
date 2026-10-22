/*
 * A368889  a(n) = Sum_{k=0..floor(n/2)} n^(3*k) * binomial(n-k,k).
 * https://oeis.org/A368889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} n^(3*k) * binomial(n-k,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 2;
    seq_t sum  = 0;
    seq_t p    = 1;                     /* current power of n^3^k   */
    seq_t nSeq = (seq_t)n;
    seq_t n3   = nSeq * nSeq * nSeq;     /* n^3                      */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t bin = binomial(n - k, k);  /* binomial(n-k,k) */
        sum += p * bin;
        if (k != maxk)                  /* prepare next power  */
            p *= n3;
    }
    return sum;
}
