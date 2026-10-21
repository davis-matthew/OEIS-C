/*
 * A281596  a(n) = ((n-2)^n - 2*(n-1)^n + n^n)/2.
 * https://oeis.org/A281596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n-2)^n - 2*(n-1)^n + n^n)/2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t nseq = (seq_t)n;

    /* Compute the three powers: n^n, (n-1)^n, (n-2)^n */
    seq_t pow_n    = big_pow(nseq,           nseq);
    seq_t pow_nm1  = big_pow(nseq - (seq_t)1, nseq);
    seq_t pow_nm2  = big_pow(nseq - (seq_t)2, nseq);

    /* Combine to avoid intermediate underflow */
    seq_t sum = pow_n + pow_nm2;          /* n^n + (n-2)^n */
    seq_t sub = pow_nm1 << 1;             /* 2*(n-1)^n  */
    seq_t temp = sum - sub;               /* ((n-2)^n - 2*(n-1)^n + n^n) */
    return temp / 2;                      /* divide by 2 */
}
