/*
 * A296185  Numbers that are not the sum of 3 squares and an 8th power.
 * https://oeis.org/A296185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296185: numbers of the form 4^(4k+2)*(8b+7). */
/* a(n) returns the n-th term for n ≥ 1. */
#include <stdbool.h>

/* Count the numbers ≤ X that are of the form
   4^(4k+2)*(8b+7).  The count is capped by 'limit' to stop
   early if we already know it exceeds 'limit'.  */
static inline idx_t
count_leq(seq_t X, idx_t limit)
{
    idx_t    cnt  = 0;
    seq_t    pow4 = 16;                    /* 4^(4*0+2) */

    while (true) {
        seq_t start = pow4 * 7;            /* smallest term for this k */
        if (start > X) break;

        seq_t step = pow4 * 8;            /* step between consecutive terms */
        idx_t  add = (idx_t)((X - start) / step + 1);
        cnt += add;
        if (cnt >= limit) return limit;

        /* move to next k (multiply by 4^4 = 256) */
        if (pow4 > X / 256) break;        /* next base would already exceed X */
        pow4 *= 256;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* for safety, though offset is 1 */

    /* first element */
    seq_t  low  = 112;
    /* rough upper bound that must contain the n-th term
       (works for all supported seq_t sizes). */
    seq_t  high = 112 + (seq_t)((idx_t)(n - 1) * 128);

    /* If the rough bound is still too small (possible when seq_t
       is very small), expand it until enough terms are counted. */
    while (count_leq(high, n) < n)
        high += 128;

    /* binary search for the smallest X with count_leq(X) ≥ n. */
    while (low < high) {
        seq_t mid = (low + high) >> 1;
        if (count_leq(mid, n) >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;  /* low == high is the n-th term */
}
