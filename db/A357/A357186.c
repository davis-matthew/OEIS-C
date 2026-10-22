/*
 * A357186  Take the k-th composition in standard order for each part k of the
 *          n-th composition in standard order, then add up everything.
 * https://oeis.org/A357186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p in C_n} (floor_log2(p)+1) where C_n is the n-th
 *            composition in standard (graded reverse‑lexicographic) order
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   compose_parts_recursive, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The code uses the mapping from a non‑negative integer to a composition
    described on OEIS A066099.  A composition of weight w has
       2^(w-1)
    members.  The compositions of weight w are in reverse‑lexicographic
    order, which can be generated recursively by choosing the last part
    from w down to 1.  The weight of the composition at index k is
             floor_log2(k) + 1    (for k>0; index 0 corresponds to the
    empty composition).

    The algorithm therefore:
       1. Determine the weight w of the n‑th composition.
       2. Find its offset pos among the 2^(w-1) compositions of weight w.
       3. Build the composition by recursion, keeping only the list of parts.
       4. For every part p add floor_log2(p)+1 to the answer.

    The implementation stores at most 64 parts (enough for n up to 2^63-1)
    and works for any n that fits in idx_t. */

#include "../utils/floor_log2_uint64.h"
#include "../utils/compose_parts_recursive.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* the empty composition */
    /* Determine the weight of the target composition.  weight = floor_log2(n) + 1. */
    int w = (int)(floor_log2_uint64(n) + 1);
    /* Compute the offset within the compositions of weight w. */
    idx_t start = (idx_t)1 << (w - 1);
    idx_t pos = n - start;                /* 0‑based within weight w */

    /* Build the composition.  At most 64 parts are needed. */
    int parts[64];
    int len = 0;
    compose_parts_recursive(w, pos, parts, &len);

    /* Sum the weights of the compositions at indices equal to the parts. */
    seq_t sum = 0;
    for (int i = 0; i < len; ++i) {
        int p = parts[i];                 /* p >= 1 */
        seq_t part_weight = (seq_t)(floor_log2_uint64((idx_t)p) + 1);
        sum += part_weight;
    }
    return sum;
}
