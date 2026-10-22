/*
 * A364746  Irregular triangle read by rows T(n,k), n >= 1, 1 <= k <=
 *          A002378(n), which is mentioned in the conjecture of A364639 (see
 *          Comments lines for definition).
 * https://oeis.org/A364746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) is the nth element in the concatenation of rows of the
 *            irregular triangle described in A364746. Each row r has r blocks
 *            each of length r+1; block m contains r-m zeros, 1, m-1 zeros,
 *            and -1 (except the last block, where the last term is 0). The
 *            global sequence is produced by reading the rows...
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: idx_t and seq_t are defined by the build environment. */
#include <stdint.h>

/* sum of terms up to row r:  r(r+1)(r+2)/3.  Use 128‑bit for safety. */
static inline __int128 cum_terms(idx_t r) {
    __int128 rr = r;
    return rr * (rr + 1) * (rr + 2) / 3;
}

seq_t a(idx_t n) {
    /* Find the row r such that cum_terms(r) >= n.  Use binary search. */
    idx_t r_low = 1;
    idx_t r_high = 1;
    __int128 target = n;             /* 128‑bit for comparison */
    while (cum_terms(r_high) < target)
        r_high <<= 1;                /* double until we are sure r_high is large enough */

    while (r_low < r_high) {
        idx_t mid = r_low + ((r_high - r_low) >> 1);
        if (cum_terms(mid) >= target)
            r_high = mid;
        else
            r_low = mid + 1;
    }
    idx_t r = r_low;                 /* the correct row index */
    __int128 prev_sum = cum_terms(r - 1);
    idx_t pos = n - (idx_t)prev_sum; /* 1‑based position within the row */

    /* Each block has length r+1 */
    idx_t block = (pos - 1) / (r + 1) + 1;          /* 1‑based block number m */
    idx_t t = (pos - 1) % (r + 1) + 1;              /* position inside the block */

    /* Compute the value according to the block definition */
    if (t <= r - block)          /* leading zeros */
        return (seq_t)0;
    if (t == r - block + 1)      /* the single '1' */
        return (seq_t)1;
    if (t <= r)                  /* the middle zeros */
        return (seq_t)0;
    /* t == r+1: the final element of the block */
    if (block == r)              /* last block of the row */
        return (seq_t)0;         /* last element is 0 */
    else
        return (seq_t)-1;        /* otherwise last element is -1 */
}
