/*
 * A375125  Strictly increasing run-leader transformation for standard
 *          compositions.
 * https://oeis.org/A375125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the leaders of the strictly increasing runs of
 *            the composition of n in graded reverse‑lexicographic (A066099)
 *            order. The algorithm uses the binary representation of n to
 *            build the composition, extracts the leaders of its increasing
 *            runs, and finally converts that leaders compositi...
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* The maximum number of parts that can appear for any idx_t
       within the limits of this implementation is 64, which covers
       any 64‑bit integer.  It also covers indices up to ~10⁴ easily. */
    enum { MAX_LEN = 64 };
    size_t pos[ MAX_LEN ];
    seq_t comp[ MAX_LEN ];
    seq_t leaders[ MAX_LEN ];
    size_t np = 0, nl = 0;

    /* --- 1. Build the composition of index n --- */
    idx_t tmp = n;
    idx_t bit_idx = 1;                 /* 1‑based bit positions */
    while (tmp) {
        if (tmp & (idx_t)1) {
            if (np >= MAX_LEN) return (seq_t)0; /* overflow guard */
            pos[ np++ ] = bit_idx;
        }
        tmp >>= 1;
        ++bit_idx;
    }
    /* With np == 0 we have the empty composition (n == 0). */
    for (size_t i = 0; i < np; ++i) {
        seq_t part = (i == 0) ? pos[0] : pos[i] - pos[i-1];
        comp[ np-1-i ] = part;      /* reverse the parts list */
    }

    /* --- 2. Extract leaders of strictly increasing runs --- */
    if (np > 0) {
        leaders[ nl++ ] = comp[0];
        for (size_t i = 1; i < np; ++i) {
            if (comp[i] > comp[i-1]) {
                /* still in the same run; no new leader */
            } else {
                leaders[ nl++ ] = comp[i];
            }
        }
    }

    /* --- 3. Convert leaders composition back to an index --- */
    idx_t index = 0;
    seq_t sum = 0;
    for (size_t i = nl; i-- > 0;) {    /* iterate leaders in reverse order */
        sum += leaders[i];
        idx_t bitpos = (idx_t)(sum - 1);   /* positions are 1‑based */
        index |= ((idx_t)1 << bitpos);
    }

    return (seq_t)index;
}
