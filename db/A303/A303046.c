/*
 * A303046  Number of minimum total dominating sets in the n-Moebius ladder.
 * https://oeis.org/A303046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(6k) = 3 * a(6k+1) = (6k+1)^2 * (k+1)^2 * a(6k+2) = (6k+2) *
 *            (4k+3) * a(6k+3) = 9 * a(6k+4) = (6k+4) * 2 * a(6k+5) = (6k+5)^2
 * Range:     u8 n <= 12, u16 n <= 36, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset is 1, unused value */

    idx_t r = n % 6;                       /* remainder modulo 6   */
    idx_t k = (n - r) / 6;                 /* integer division    */
    seq_t nseq = (seq_t)n;                 /* cast to sequence type */
    seq_t kseq = (seq_t)k;                 /* cast to sequence type */

    switch (r)
    {
        case 0:                            /* a(6k)   */
            return (seq_t)3;

        case 1: {                          /* a(6k+1) */
            seq_t temp = kseq + 1;         /* (k+1)   */
            return nseq * nseq * temp * temp;
        }

        case 2: {                          /* a(6k+2) */
            seq_t part = (kseq << 2) + 3; /* (4k+3)  */
            return nseq * part;
        }

        case 3:                            /* a(6k+3) */
            return (seq_t)9;

        case 4:                            /* a(6k+4) */
            return nseq * (seq_t)2;

        case 5:                            /* a(6k+5) */
            return nseq * nseq;
    }

    return 0; /* unreachable, but keeps compiler happy */
}
