/*
 * A367795  Triangle read by rows, where row n = L(n) is defined by L(1) =
 *          [1,0] and L(n+1) is obtained from L(n) by inserting their binary
 *          concatenation between elements x,y.
 * https://oeis.org/A367795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   bitlen_seq_t, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Triangle A367795:  The sequence a(n) is obtained by reading
   triangle rows L(n) row‑by‑row.  L(1) = [1,0] and L(n+1) is
   constructed from L(n) by inserting between each adjacent pair (x,y)
   the binary concatenation of x and y.  For example:
       L(2) = [1, 2, 0]
       L(3) = [1, 6, 2, 4, 0]
   The function computes a(n) for arbitrary n ≥ 1 by constructing
   rows up to the required one.  It works for all supported seq_t
   (8, 16, 32, 64, 128 bits or an arbitrary‑precision type) and
   for all idx_t (uint64_t) indices.  Time is O(log n) per call.
 */

#include <stdlib.h>
#include <stdint.h>

#include "../utils/bitlen_seq_t.h"   /* bitlen_seq_t() */
#include "../utils/pow2.h"           /* pow2() */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* Locate the row that contains position n.  The length of row r is
       len(r) = 2^(r-1) + 1.  The cumulative number of elements up to
       row r is tot(r) = 2^r - 1 + r. */
    idx_t r     = 1;
    idx_t cum   = 0;  /* sum of lengths of rows before r */
    idx_t len   = 2;  /* length of current row */

    /* Find the smallest r such that cum + len >= n */
    while (cum + len < n) {
        cum += len;
        ++r;
        /* len(r) = 2^(r-1) + 1;  r is small (≤64 for 64‑bit indices) */
        len = ((idx_t)1ULL << (r - 1)) + (idx_t)1;
    }

    /* Index within the found row (0‑based) */
    idx_t k = n - cum - 1;

    /* Build rows sequentially up to row r.  Use dynamic allocation
       that is compatible with both C and C++. */
    idx_t prev_len = 2;  /* length of L(1) */
    seq_t *prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
    prev[0] = (seq_t)1;
    prev[1] = (seq_t)0;

    for (idx_t row = 2; row <= r; ++row) {
        idx_t curr_len = 2 * prev_len - 1;
        seq_t *curr = (seq_t *)calloc(curr_len, sizeof(seq_t));

        idx_t pos = 0;
        for (idx_t i = 0; i < prev_len; ++i) {
            curr[pos++] = prev[i];          /* copy element */
            if (i + 1 < prev_len) {
                seq_t x = prev[i];
                seq_t y = prev[i + 1];
                /* 0 is considered 1‑bit long */
                unsigned bits_y = (y == (seq_t)0) ? 1 : bitlen_seq_t(y);
                seq_t concat = x * pow2(bits_y) + y;
                curr[pos++] = concat;      /* insert concatenation */
            }
        }

        free(prev);
        prev      = curr;
        prev_len  = curr_len;
    }

    seq_t result = prev[k];
    free(prev);
    return result;
}
