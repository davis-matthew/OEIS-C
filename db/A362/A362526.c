/*
 * A362526  a(n) = 2^n*(n + 2) + (n - 7)*n/2 - 2.
 * https://oeis.org/A362526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(n + 2) + (n - 7)*n/2 - 2
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset compliance */
    seq_t nseq = (seq_t)n;
    seq_t term1 = pow2(n) * (nseq + (seq_t)2); /* 2^n * (n + 2)   */
    seq_t result;
    if (n >= 7) {
        seq_t term2 = (nseq * (nseq - (seq_t)7)) / (seq_t)2; /* (n-7)*n/2  */
        result = term1 + term2 - (seq_t)2;
    } else {
        seq_t sub = ((seq_t)(7 - n) * nseq) / (seq_t)2;       /* -(7-n)*n/2 */
        result = term1 - sub - (seq_t)2;
    }
    return result;
}
