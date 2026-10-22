/*
 * A334151  Numbers k such that k / rad(k) > m / rad(m) for all m < k.
 * https://oeis.org/A334151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 35, u64 n <= 79, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A334151
   Record values of k/rad(k); those numbers k for which k/rad(k) is larger
   than for all m < k.  The sequence consists of powers of 2 together
   with selected powers of 3 (only those 3^e whose ratio 3^(e-1) exceeds
   the current maximum record ratio).  The implementation generates
   the terms on demand by iterating over powers of 2 and powers of 3
   in ascending value and keeping a running record of the current
   maximum ratio. */

#include <stdbool.h>

static const seq_t TWO  = (seq_t)2;
static const seq_t THREE = (seq_t)3;

/* Return the n-th term of sequence A334151 (1‑based).  */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;           /* the first term is 1 */

    idx_t  count = 1;                      /* terms already found (k=1) */
    seq_t  max_ratio = (seq_t)1;            /* ratio of the last record */
    seq_t  pow2  = (seq_t)4;               /* next power of 2 to test */
    seq_t  pow3  = (seq_t)3;               /* next power of 3 to test */
    seq_t  result = (seq_t)0;              

    while (count < n)
    {
        if (pow2 <= pow3)
        {
            /* candidate is a power of 2 */
            seq_t ratio = pow2 / TWO;      /* k/rad(k) for k=pow2 */
            if (ratio > max_ratio)
            {
                max_ratio = ratio;
                ++count;
                if (count == n) result = pow2;
            }
            pow2 *= TWO;                   /* next power of 2 */
        }
        else
        {
            /* candidate is a power of 3 */
            seq_t ratio = pow3 / THREE;    /* k/rad(k) for k=pow3 */
            if (ratio > max_ratio)
            {
                max_ratio = ratio;
                ++count;
                if (count == n) result = pow3;
            }
            pow3 *= THREE;                 /* next power of 3 */
        }
    }

    return result;
}
