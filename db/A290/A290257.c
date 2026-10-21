/*
 * A290257  a(n) is the size of the Durfee square of the integer partition
 *          having viabin number n.
 * https://oeis.org/A290257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A290257:
   a(n) = max k such that the first k 1's in the binary representation
   of 2n precede the last k 0's.  For n with no 0's in 2n, a(n)=1.  */
/* Requires <stddef.h> for size_t. */

#include <stddef.h>
#include <stdbool.h>

/* External types are supplied by the build. */
seq_t a(idx_t n)
{
    /* 2n as idx_t.  n is unsigned so shift is safe for the ranges tested. */
    idx_t m = n << 1U;

    /* Determine the number of bits in m (at least 1). */
    idx_t tmp = m;
    idx_t len = 0;
    while (tmp != 0U) {
        len++;
        tmp >>= 1U;
    }
    /* m==0 would give len==0, but that only happens for n==0 which is
       outside the offset; return 1 safely. */
    if (len == 0) {
        return (seq_t)1;
    }

    /* Record positions of 1s and 0s from most significant to least. */
    int ones[64];
    int zeros[64];
    int oc = 0, zc = 0;

    for (int i = (int)len - 1; i >= 0; --i) {
        if ((m >> i) & 1U) {
            ones[oc++] = i;
        } else {
            zeros[zc++] = i;
        }
    }

    /* The maximum possible k is limited by the smaller count. */
    int kmax = (oc < zc) ? oc : zc;
    int result = 1;   /* a(n) is at least 1 for all n ≥ 1 */

    for (int k = 1; k <= kmax; ++k) {
        /* ones[k-1] is the rightmost of the first k ones.
           zeros[zc-k] is the leftmost of the last k zeros.  */
        if (ones[k - 1] > zeros[zc - k]) {
            result = k;
        } else {
            break; /* further k will not satisfy the condition */
        }
    }

    return (seq_t)result;
}
