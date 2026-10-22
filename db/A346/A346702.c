/*
 * A346702  The a(n)-th composition in standard order is the odd bisection of
 *          the n-th composition in standard order.
 * https://oeis.org/A346702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) is the index of the odd‑bisection of the n‑th composition
 *            (in the graded reverse‑lexicographic order of all compositions,
 *            the first composition being the empty one). Algorithm: 1. Find
 *            the sum m of the n-th composition: 2^(m-1) ≤ n < 2^m (m=0 only
 *            for n=0). 2. Represent the offset within the...
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* --- Step 1: determine the sum m of the n-th composition --- */
    idx_t m;
    if (n == 0) {
        m = 0;          /* the empty composition */
    } else {
        /* m = number of bits in n (i.e., floor_log2(n)+1) */
        idx_t t = n;
        m = 1;
        while (t >>= 1) {
            ++m;
        }
    }

    /* --- Step 2: compute offset within compositions of sum m --- */
    idx_t offset;
    if (m == 0) {
        offset = 0;
    } else {
        offset = n - ((idx_t)1 << (m - 1));   /* 2^(m-1) <= n < 2^m */
    }

    /* --- Step 3: build parts of the n-th composition --- */
    if (m == 0) {
        /* Empty composition => odd bisection is empty => a(n)=0 */
        return (seq_t)0;
    }

    /* maximum possible parts is m (<= 64 for uint64_t). */
    idx_t parts[64];
    idx_t part_cnt = 0;
    idx_t last_sep = 0;

    /* iterate through possible separator positions i = 1 .. m-1 */
    for (idx_t i = 1; i <= m - 1; ++i) {
        /* bit (m-1-i) of offset indicates separator after element i */
        if ( (offset >> (m - 1 - i)) & 1 ) {
            parts[part_cnt++] = i - last_sep;
            last_sep = i;
        }
    }
    /* final part: from last separator to end */
    parts[part_cnt++] = m - last_sep;

    /* --- Step 4: collect odd positions parts --- */
    idx_t odd_parts[64];
    idx_t odd_cnt = 0;
    for (idx_t j = 0; j < part_cnt; j += 2) {
        odd_parts[odd_cnt++] = parts[j];
    }

    /* sum of odd parts gives the new sum m_odd */
    idx_t sum_odd = 0;
    for (idx_t j = 0; j < odd_cnt; ++j) {
        sum_odd += odd_parts[j];
    }

    if (sum_odd == 0) {
        return (seq_t)0;   /* empty odd bisection */
    }

    /* --- Step 5: compute rank of the odd composition --- */
    /* Build offset for the odd composition */
    idx_t offset_q = 0;
    idx_t cumulative = 0;          /* cumulative sum of processed odd parts */
    for (idx_t k = 0; k < odd_cnt; ++k) {
        cumulative += odd_parts[k];
        if (k < odd_cnt - 1) {
            /* separator after element at position cumulative */
            idx_t bit_pos = sum_odd - 1 - cumulative;
            offset_q |= ((idx_t)1 << bit_pos);
        }
    }

    /* rank = 2^(sum_odd-1) + offset_q  (for sum_odd ≥ 1) */
    return (seq_t)((((seq_t)1) << (sum_odd - 1)) + (seq_t)offset_q);
}
