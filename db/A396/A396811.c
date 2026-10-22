/*
 * A396811  Lower (1,1/2) midsequence of (floor[n/2]^2) and (ceiling[n/2]^2);
 *          see Comments.
 * https://oeis.org/A396811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A396811 : Lower (1,1/2) midsequence of floor(n/2)^2 and ceil(n/2)^2
 *  Formula: a(n) = floor( floor(n/2)^2 + ceil(n/2)^2 / 2 )
 *  (with floor and ceil using integer division, a(0)=0)
 */
#include <stdint.h>

/* Compute a(n) for any nonnegative n. */
seq_t a(idx_t n)
{
    /* Compute floor and ceil of n/2 without overflow */
    idx_t k = n >> 1;               /* floor(n/2) */
    idx_t l = k + (n & 1U);         /* ceil(n/2) = (n+1)/2 */

    /* Convert to seq_t and compute squares */
    seq_t kseq = (seq_t)k;
    seq_t lseq = (seq_t)l;
    seq_t s = kseq * kseq;          /* floor(n/2)^2 */
    seq_t t = lseq * lseq;          /* ceil(n/2)^2 */

    /* a(n) = s + floor(t/2) */
    return s + (t / 2);
}
